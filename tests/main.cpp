#include <future>
#include <regex>
#include <string>

#include "pposix/dlfcn.hpp"
#include "pt.hpp"

PT_TEST(pt::tests, test_hello_main) { PT_ASSERT(false); }

struct TestCase {
  virtual ~TestCase() = default;

  TestCase() = default;

  TestCase(const TestCase &) = delete;
  TestCase(TestCase &&) = delete;

  TestCase &operator=(const TestCase &) = delete;
  TestCase &operator=(TestCase &&) = delete;

  inline virtual const pt::Test &test() const {
    throw std::logic_error{"pt::Test::test should never be called."};
  };

  virtual std::future<pt::RunResult> run() const {
    throw std::logic_error{"pt::Test::run should never be called."};
  }
};

class NormalTest final : public TestCase {
 public:
  inline explicit NormalTest(const pt::Test &test) : test_{test} {}

  inline const pt::Test &test() const override { return test_; }

  inline std::future<pt::RunResult> run() const override {
    std::promise<pt::RunResult> p{};
    p.set_value(pt::RunResult{});
    return p.get_future();
  }

 private:
  const pt::Test &test_;
};

class ThreadTest final : public TestCase {
 public:
  inline explicit ThreadTest(const pt::Test &test) : test_{test} {}

  inline const pt::Test &test() const override { return test_; }

  inline std::future<pt::RunResult> run() const override {
    std::promise<pt::RunResult> p{};
    p.set_value(pt::RunResult{});
    return p.get_future();
  }

 private:
  const pt::Test &test_;
};

class SpawnTest final : public TestCase {
 public:
  inline explicit SpawnTest(const pt::Test &test) : test_{test} {}

  inline const pt::Test &test() const override { return test_; }

  inline std::future<pt::RunResult> run() const override {
    std::promise<pt::RunResult> p{};
    p.set_value(pt::RunResult{});
    return p.get_future();
  }

 private:
  const pt::Test &test_;
};

std::unique_ptr<TestCase> wrap_test(const pt::Test &test) {
  switch (test.type()) {
    case pt::Type::Normal:
      return std::make_unique<NormalTest>(test);
    case pt::Type::Thread:
      return std::make_unique<ThreadTest>(test);
    case pt::Type::Spawn:
      return std::make_unique<SpawnTest>(test);
    default:
      throw std::logic_error{"Unhandled test type: " +
                             std::to_string(static_cast<pt::capi::pt_test_type_t>(test.type()))};
  }
}

[[noreturn]] void run_one(const std::string &name) {
  for (auto test_entry{pt::private_detail::internal_tests()}; test_entry;
       test_entry = test_entry->next()) {
    const auto full_name{test_entry->id().full_name()};
    if (full_name == name) {
      try {
        pt::run(*test_entry);
        std::exit(EXIT_SUCCESS);
      } catch (...) {
        std::cerr << "INTERNAL ERROR: Unknown exception caught in pt::run_one!" << std::endl;
        std::exit(EXIT_FAILURE);
      }
    }
  }

  std::cerr << "ERROR: Test case not found: " << name << std::endl;
  std::exit(EXIT_FAILURE);
}

[[noreturn]] void run_matching(const std::vector<std::regex> &patterns) {
  try {
    std::vector<std::reference_wrapper<const pt::Test>> tests{};
    size_t total_tests{};

    for (auto test_entry{pt::private_detail::internal_tests()}; test_entry;
         test_entry = test_entry->next()) {
      ++total_tests;

      const auto test_id{test_entry->id()};

      const auto full_name{test_id.full_name()};
      for (const auto &pattern : patterns) {
        if (std::regex_match(full_name, pattern)) {
          tests.emplace_back(*test_entry);
          break;
        }
      }
    }

    std::cout << "Found " << total_tests << " tests" << std::endl;
    std::cout << "Matched " << tests.size() << " tests" << std::endl;

    for (const auto &test : tests) {
      pt::run(test);
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
  if (const auto res = pposix::capi::dlopen("./libtest_pposix.so")) {
    auto pt_symbol_table = pposix::capi::dlsym((*res).raw(), "pt_symbol_table");
    PT_ASSERT(!pt_symbol_table.has_error());

    const auto handle{std::move(pt_symbol_table).value()};

    auto const *symbols = static_cast<pt::capi::PtSymbolTable const *>(
        static_cast<pposix::capi::raw_sym_t>(handle.raw()));

    const auto &secret{symbols->id.secret};

    if (secret[0u] != 'p' || secret[1u] != 't' || secret[2u] != 'l' || secret[3u] != 's') {
      std::cout << "Invalid symbol table secret" << std::endl;
    } else {
      size_t count{};
      for (auto entry = symbols->pt_test_entries(); !symbols->pt_test_entries_stop(entry).val;
           entry = symbols->pt_test_entries_next(entry)) {
        symbols->pt_test_entry_run(entry);
        count++;
      }

      PT_ASSERT(symbols->pt_test_entries_count().val == count);
    }

  } else {
    std::cerr << "No library tests found" << std::endl;
  }

  for (auto test_entry{pt::private_detail::internal_tests()}; test_entry;
       test_entry = test_entry->next()) {
    try {
      pt::run(*test_entry);
    } catch (...) {
      std::cerr << "INTERNAL ERROR: Unknown exception caught in pt::run_all!";
      std::cerr << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }

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