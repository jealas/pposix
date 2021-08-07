#include <dlfcn.h>

#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <memory>
#include <mutex>
#include <queue>
#include <regex>
#include <string>

#include "pt.hpp"

static const pt::capi::PtStreamVTable fprint_stream_vtable{
    +[](pt::capi::PtStreamData data, char const *const str) {
      std::fputs(str, static_cast<FILE *>(data.val));
    },
};

static const pt::capi::PtLoggerVTable fprint_logger_vtable{
    +[](pt::capi::PtLoggerData, pt::capi::PtLogLevel level, char const *str) noexcept {
      const auto file_stream{[level]() {
        switch (level.val) {
          case pt::capi::pt_log_level::info:
          case pt::capi::pt_log_level::warning:
            return stdout;

          case pt::capi::pt_log_level::error:
          case pt::capi::pt_log_level::fatal:
          default:
            return stderr;
        }
      }()};

      ::fputs(str, file_stream);
    },
    +[](pt::capi::PtLoggerData, pt::capi::PtLogLevel level) noexcept {
      const auto file_stream{[level]() {
        switch (level.val) {
          case pt::capi::pt_log_level::info:
          case pt::capi::pt_log_level::warning:
            return stdout;

          case pt::capi::pt_log_level::error:
          case pt::capi::pt_log_level::fatal:
          default:
            return stderr;
        }
      }()};

      return pt::capi::PtLogStream{
          &fprint_stream_vtable,
          {
              static_cast<void *>(file_stream),
          },
      };
    },
};

template <class T>
struct WorkQueue {
  std::shared_ptr<T> wait_pop(const std::chrono::steady_clock::time_point deadline) {
    std::unique_lock<std::mutex> lk{m_};
    if (cv_.wait_until(lk, deadline, [this]() { return !q_.empty(); })) {
      auto front{std::move(q_.front())};
      q_.pop();
      return std::move(front);
    } else {
      return {};
    }
  }

  std::shared_ptr<T> try_pop() {
    std::lock_guard<std::mutex> lk{m_};

    if (q_.empty()) {
      return {};
    } else {
      auto front{std::move(q_.front())};
      q_.pop();
      return std::move(front);
    }
  }

  void push(std::shared_ptr<T> value) {
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

  inline pt::RunResult run(pt::Logger logger) const noexcept {
    return pt::RunResult(syms_->pt_test_entry_run(test_, logger.c_log()).val);
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

const auto close_lib{[](void *lib) { ::dlclose(lib); }};

std::unique_ptr<void, decltype(close_lib)> load_test_lib(char const *const path) {
  if (auto lib_handle{::dlopen(path, RTLD_GLOBAL | RTLD_LAZY)}) {
    return {lib_handle, close_lib};
  } else {
    return {nullptr, close_lib};
  }
}

std::shared_ptr<const pt::capi::PtSymbolTable> load_symbol_table(char const *const path) noexcept {
  if (auto lib{load_test_lib(path)}) {
    if (const auto symbol_handle = ::dlsym(lib.get(), PT_SYMBOL_TABLE_NAME_STR)) {
      // Transfer ownership of the underlying symbol handle to the symbols pointer
      auto const *symbols = static_cast<pt::capi::PtSymbolTable const *>(symbol_handle);
      const auto &secret{symbols->id.secret.val};

      if (secret[0u] != 'p' || secret[1u] != 't' || secret[2u] != 'l' || secret[3u] != 's') {
        return {};

      } else {
        return std::shared_ptr<const pt::capi::PtSymbolTable>{
            symbols,
            [lib_descriptor = lib.release()](const pt::capi::PtSymbolTable *) mutable noexcept {
              close_lib(lib_descriptor);
            }};
      }
    } else {
    }
  }

  return {};
}

[[noreturn]] void run_one(const std::string &name) {
  std::fprintf(stdout, "Run one\n");

  pt::Logger logger{pt::capi::PtLogger{&fprint_logger_vtable, {nullptr}}};

  for_each_internal_test([&](const auto test_entry) {
    const auto &test{test_entry.get()};

    const auto full_name{test.id().full_name()};
    if (full_name == name) {
      const auto result{pt::private_detail::run_internal(test, logger)};
      if (result == pt::RunResult::Success) {
        std::exit(EXIT_SUCCESS);
      } else {
        std::exit(EXIT_FAILURE);
      }
    }
  });

  std::fprintf(stderr, "ERROR: Test case not found: %s", name.c_str());
  std::exit(EXIT_FAILURE);
}

[[noreturn]] void run_matching(const std::vector<std::regex> &patterns) {
  std::fprintf(stdout, "Run matching\n");

  pt::Logger logger{pt::capi::PtLogger{&fprint_logger_vtable, {nullptr}}};

  if (const auto symbols{load_symbol_table("./libtest_compiles.so")}) {
    size_t count{};
    for_each_library_test(*symbols, [&](const auto test) {
      count++;

      const pt::Id id{symbols->pt_test_entry_namespace(test), symbols->pt_test_entry_name(test)};
      for (const auto &pattern : patterns) {
        if (std::regex_search(id.full_name(), pattern)) {
          symbols->pt_test_entry_run(test, logger.c_log());
          break;
        }
      }
    });
  }

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

    std::fprintf(stdout, "Found %zu tests", total_tests);
    std::fprintf(stdout, "Matched %zu tests", tests.size());

    pt::Logger logger{pt::capi::PtLogger{&fprint_logger_vtable, {nullptr}}};
    for (const auto &test : tests) {
      pt::private_detail::run_internal(test, logger);
    }
  } catch (const std::exception &exception) {
    std::fprintf(stderr, "INTERNAL ERROR: Uncaught exception in pt::run_matching: %s",
                 exception.what());

    std::exit(EXIT_FAILURE);
  } catch (...) {
    std::fprintf(stderr, "INTERNAL ERROR: Unknown exception caught in pt::run_matching!");

    std::exit(EXIT_FAILURE);
  }

  std::exit(EXIT_FAILURE);
}

[[noreturn]] void run_all() {
  std::fprintf(stdout, "Run all\n");

  pt::Logger logger{pt::capi::PtLogger{&fprint_logger_vtable, {nullptr}}};

  if (const auto symbols{load_symbol_table("./libtest_compiles.so")}) {
    for_each_library_test(
        *symbols, [&](const auto test) { symbols->pt_test_entry_run(test, logger.c_log()); });
  }

  for_each_internal_test([&](const auto test) { pt::private_detail::run_internal(test, logger); });

  std::exit(EXIT_SUCCESS);
}

int main(const int argc, const char *argv[]) {
  if (argc <= 1) {
    run_all();

  } else if (std::strcmp(argv[1], "run") == 0) {
    if (argc != 3) {
      std::fprintf(stderr, "'run' command only accepts one test name");
      std::exit(EXIT_FAILURE);
    }

    run_one(argv[2]);

  } else {
    std::vector<std::regex> patterns{};

    for (auto i{1}; i < argc; ++i) {
      const auto regex_c_str{argv[i]};

      try {
        patterns.emplace_back(regex_c_str);
      } catch (const std::regex_error &error) {
        std::fprintf(stderr, "Invalid test name regex: \"%s\"\nstd::regex::error %s\n",
                     regex_c_str, error.what());
        std::exit(EXIT_FAILURE);
      }
    }

    run_matching(patterns);
  }

  std::exit(EXIT_FAILURE);
}