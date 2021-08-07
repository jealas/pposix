#include <dlfcn.h>

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <regex>
#include <string>

#include "pt.hpp"

namespace my_namespace {

PT_SUITE("pt main") {
  PT_TEST(hello_main) {
    PT_ASSERT(true);
    PT_ASSERT(true);
    PT_ASSERT(true);

    PT_ASSERT(false);
  }
}

}  // namespace my_namespace

template <class T>
struct WorkQueue {
  std::shared_ptr<T> WaitUntilPop(const std::chrono::steady_clock::time_point deadline) {
    std::unique_lock<std::mutex> lk{m_};
    if (cv_.wait_until(lk, deadline, [this]() { return !q_.empty(); })) {
      auto front{std::move(q_.front())};
      q_.pop();
      return std::move(front);
    } else {
      return {};
    }
  }

  std::shared_ptr<T> TryPop() {
    std::lock_guard<std::mutex> lk{m_};

    if (q_.empty()) {
      return {};
    } else {
      auto front{std::move(q_.front())};
      q_.pop();
      return std::move(front);
    }
  }

  void Push(std::shared_ptr<T> value) {
    {
      std::lock_guard<std::mutex> lk{m_};
      q_.push(std::move(value));
    }

    cv_.notify_one();
  }

  mutable std::mutex m_{};
  std::condition_variable cv_{};
  std::queue<std::shared_ptr<T>> q_{};
};

struct TestCase {
  virtual ~TestCase() = default;

  TestCase() = default;

  TestCase(const TestCase &) = delete;
  TestCase(TestCase &&) = delete;

  TestCase &operator=(const TestCase &) = delete;
  TestCase &operator=(TestCase &&) = delete;
};

class NormalTest final : public TestCase {};

class LibraryTest final : public TestCase {
 public:
  inline explicit LibraryTest(std::shared_ptr<pt::capi::PtSymbolTable> syms,
                              pt::capi::PtTestEntry test)
      : syms_{std::move(syms)}, test_{test} {
    assert(syms_);
  };

  inline pt::Id id() const noexcept {
    return pt::Id{syms_->pt_test_entry_namespace(test_), syms_->pt_test_entry_name(test_)};
  }

  inline pt::Location loc() const noexcept {
    return pt::Location{syms_->pt_test_entry_file(test_), syms_->pt_test_entry_line(test_)};
  }

  inline pt::RunResult run() const noexcept {
    return pt::RunResult(syms_->pt_test_entry_run(test_).val);
  }

 private:
  std::shared_ptr<pt::capi::PtSymbolTable> syms_{};
  pt::capi::PtTestEntry test_;
};

class SpawnTest final : public TestCase {};

template <class Fn>
void for_each_library_test(const pt::capi::PtSymbolTable &syms, Fn fn) {
  for (auto test{syms.pt_normal_tests()}; !syms.pt_test_entries_stop(test).val;
       test = syms.pt_test_entries_next(test)) {
    fn(test);
  }
}

template <class Fn>
void for_each_internal_test(Fn fn) {
  for (auto test{pt::private_detail::internal_tests()}; test != nullptr; test = test->next()) {
    fn(std::cref(*test));
  }
}

std::shared_ptr<const pt::capi::PtSymbolTable> load_symbol_table(const char *path) noexcept {
  if (const auto lib_handle{::dlopen(path, RTLD_GLOBAL | RTLD_LAZY)}) {
    if (const auto symbol_handle = ::dlsym(lib_handle, PT_SYMBOL_TABLE_NAME_STR)) {
      // Transfer ownership of the underlying symbol handle to the symbols pointer
      auto const *symbols = static_cast<pt::capi::PtSymbolTable const *>(symbol_handle);
      const auto &secret{symbols->id.secret.val};

      if (secret[0u] != 'p' || secret[1u] != 't' || secret[2u] != 'l' || secret[3u] != 's') {
        std::cout << "Invalid symbol table secret" << std::endl;
        return {};

      } else {
        return std::shared_ptr<const pt::capi::PtSymbolTable>{
            symbols, [lib_handle](const pt::capi::PtSymbolTable *) mutable noexcept {
              ::dlclose(lib_handle);
            }};
      }
    }
  }

  return {};
}

[[noreturn]] void run_one(const std::string &name) {
  std::cout << "Run one" << std::endl;

  for_each_internal_test([&](const auto test_entry) {
    const auto &test{test_entry.get()};

    const auto full_name{test.id().full_name()};
    if (full_name == name) {
      const auto result{pt::private_detail::run_internal(test)};
      if (result == pt::RunResult::Success) {
        std::exit(EXIT_SUCCESS);
      } else {
        std::exit(EXIT_FAILURE);
      }
    }
  });

  std::cerr << "ERROR: Test case not found: " << name << std::endl;
  std::exit(EXIT_FAILURE);
}

[[noreturn]] void run_matching(const std::vector<std::regex> &patterns) {
  std::cout << "Run matching" << std::endl;

  const auto symbols{load_symbol_table("./libtest_pposix.so")};

  size_t count{};
  for_each_library_test(*symbols, [&](const auto test) {
    count++;

    const pt::Id id{symbols->pt_test_entry_namespace(test), symbols->pt_test_entry_name(test)};
    for (const auto &pattern : patterns) {
      if (std::regex_search(id.full_name(), pattern)) {
        symbols->pt_test_entry_run(test);
        break;
      }
    }
  });

  try {
    std::vector<std::reference_wrapper<const pt::InternalTest>> tests{};
    size_t total_tests{};

    for_each_internal_test([&](const auto test_entry) {
      const auto &test{test_entry.get()};

      ++total_tests;

      const auto test_id{test.id()};

      const auto full_name{test_id.full_name()};
      for (const auto &pattern : patterns) {
        if (std::regex_search(full_name, pattern)) {
          tests.emplace_back(test);
          break;
        }
      }
    });

    std::cout << "Found " << total_tests << " tests" << std::endl;
    std::cout << "Matched " << tests.size() << " tests" << std::endl;

    for (const auto &test : tests) {
      pt::private_detail::run_internal(test);
    }

    std::exit(EXIT_SUCCESS);

  } catch (const std::exception &exception) {
    std::cerr << "INTERNAL ERROR: Uncaught exception in pt::run_matching: " << exception.what();
    std::cerr << std::endl;
    std::exit(EXIT_FAILURE);

  } catch (...) {
    std::cerr << "INTERNAL ERROR: Unknown exception caught in pt::run_matching!";
    std::cerr << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

[[noreturn]] void run_all() {
  std::cout << "Run all" << std::endl;

  const auto symbols{load_symbol_table("./libtest_pposix.so")};
  for_each_library_test(*symbols, [&](const auto test) { symbols->pt_test_entry_run(test); });

  for_each_internal_test([](const auto test) { pt::private_detail::run_internal(test); });

  std::exit(EXIT_SUCCESS);
}

int main(const int argc, const char *argv[]) {
  if (argc <= 1) {
    run_all();

  } else if (std::strcmp(argv[1], "run") == 0) {
    if (argc != 3) {
      std::cerr << "'run' command only accepts one test name" << std::endl;
      return EXIT_FAILURE;
    }

    run_one(argv[2]);

  } else {
    std::vector<std::regex> patterns{};

    for (auto i{1}; i < argc; ++i) {
      const auto regex_c_str{argv[i]};

      try {
        patterns.emplace_back(regex_c_str);
      } catch (const std::regex_error &error) {
        std::cerr << "Invalid test name regex: " << '"' << regex_c_str << '"' << '\n'
                  << "std::regex_error" << '-' << error.what() << std::endl;
        return EXIT_FAILURE;
      }
    }

    run_matching(patterns);
  }

  return EXIT_FAILURE;
}