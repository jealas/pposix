#include "pposix_test.hpp"

namespace pposix::test {

struct {
  std::unordered_map<std::string, Info> tests{};
} static registrar{};

const std::unordered_map<std::string, Info> &registered_tests() { return registrar.tests; }

void register_test(char const *const name_space, char const *const name,
                   std::function<void()> fn) {
  Info info{name_space, name, std::move(fn)};
  auto full_name{info.full_name()};

  try {
    if (registrar.tests.count(full_name)) {
      std::cerr << "FATAL ERROR: "
                << "Duplicate test name found (should never occur)" << std::endl;
      std::exit(EXIT_FAILURE);
    } else {
      registrar.tests.emplace(std::make_pair(std::move(full_name), std::move(info)));
    }
  } catch (const std::exception &exception) {
    std::cerr << "Uncaught exception while registering test " << name_space << "::" << name
              << " - " << exception.what() << '\n';
    std::cerr << std::endl;
    std::exit(EXIT_FAILURE);
  } catch (...) {
    std::cerr << "Unknown exception caught while registering test " << name_space << "::" << name
              << '\n';
    std::cerr << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

void run(const std::vector<std::reference_wrapper<const Info>> &test_matches) {
  for (const auto &test_info_ref : test_matches) {
    const auto &test_info{test_info_ref.get()};

    try {
      std::cout << "Running " << test_info << std::endl;
      test_info.fn();

    } catch (const assertion_error &error) {
      std::cerr << "FAILED: " << test_info << '\n' << error.what();
      std::cerr << std::endl;
      continue;

    } catch (const std::exception &exception) {
      std::cerr << "ERROR: Uncaught exception while running test " << test_info << '\n'
                << exception.what();

      std::cerr << std::endl;
      continue;

    } catch (...) {
      std::cerr << "FATAL:: Unknown exception caught while running test " << test_info;

      std::cerr << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }
}

[[noreturn]] void main(const std::vector<std::regex> &patterns) {
  const auto &registered_tests{pposix::test::registered_tests()};

  try {
    std::unordered_map<std::string, std::reference_wrapper<const pposix::test::Info>> tests_map{};

    std::cout << "Found " << registered_tests.size() << " tests" << std::endl;

    for (const auto &test_entry : registered_tests) {
      auto full_name{test_entry.second.full_name()};
      for (const auto &pattern : patterns) {
        if (std::regex_match(full_name, pattern)) {
          tests_map.try_emplace(std::move(full_name), test_entry.second);
        }
      }
    }

    std::vector<std::reference_wrapper<const pposix::test::Info>> tests{};
    tests.reserve(tests_map.size());

    for (const auto &match_entry : tests_map) {
      tests.emplace_back(match_entry.second);
    }

    std::cout << "Matched " << tests.size() << " tests" << std::endl;
    pposix::test::run(tests);
    std::exit(EXIT_SUCCESS);

  } catch (const std::exception &exception) {
    std::cerr << "Uncaught exception in pposix::test::main: " << exception.what();
    std::cerr << std::endl;
    std::exit(EXIT_FAILURE);

  } catch (...) {
    std::cerr << "Unknown exception caught in pposix::test::main!";
    std::cerr << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

}  // namespace pposix::test
