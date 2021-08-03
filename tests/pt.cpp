#include "pt.hpp"

#include <cassert>
#include <random>

namespace pt {

struct {
  RegistrationEntry *tail{nullptr};
} static registrar{};

RegistrationEntry const *registered_tests() { return registrar.tail; }

void register_test(RegistrationEntry &entry) {
  assert(entry.next == nullptr);

  try {
    // Only and last modification to the registration entry
    entry.next = registrar.tail;

    registrar.tail = &entry;

  } catch (...) {
    std::cerr << "Unknown exception caught while registering test " << entry.id << '\n';
    std::cerr << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

void run(const std::vector<Test> &tests) {
  for (const auto &test : tests) {
    const auto &test_id{test.id()};

    try {
      std::cout << "Running " << test_id << std::endl;
      test.run();

    } catch (const test_failed &fail) {
      std::cerr << "FAILED: " << test.id() << '\n' << '\t' << fail.what() << '\n';
      std::cerr << std::endl;
      continue;

    } catch (const std::exception &exception) {
      std::cerr << "ERROR: Uncaught exception while running test " << test_id << '\n'
                << exception.what();
      std::cerr << std::endl;
      continue;

    } catch (...) {
      std::cerr << "FATAL: Unknown exception caught while running test " << test_id;
      std::cerr << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }
}

[[noreturn]] void main(const std::vector<std::regex> &patterns) {
  try {
    std::vector<Test> tests{};
    size_t total_tests{};

    for (auto test_entry{registered_tests()}; test_entry; test_entry = test_entry->next) {
      ++total_tests;

      const auto test_id{test_entry->id.get()};

      auto full_name{test_id.full_name()};
      for (const auto &pattern : patterns) {
        if (std::regex_match(full_name, pattern)) {
          tests.push_back(Test{test_entry});
          break;
        }
      }
    }

    std::cout << "Found " << total_tests << " tests" << std::endl;
    std::cout << "Matched " << tests.size() << " tests" << std::endl;

    std::cout << "Shuffling tests" << std::endl;
    {
      std::random_device rd{};
      std::mt19937 g{rd()};
      std::shuffle(tests.begin(), tests.end(), g);
    }

    pt::run(tests);
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

}  // namespace pt
