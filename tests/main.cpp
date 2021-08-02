#include <iostream>

#include "pposix_test.hpp"

PPOSIX_TEST(pposix::tests, test_hello_world1, []() { PPOSIX_ASSERT(20 == 4); })

PPOSIX_TEST(pposix::tests, test_hello_world2, []() { PPOSIX_ASSERT(20 == 2); })

PPOSIX_TEST(pposix::tests, test_hello_world3, []() {})

int main() {
  try {
    pposix::test::main();
    std::exit(EXIT_SUCCESS);
  } catch (const std::exception &exception) {
    std::cerr << "Uncaught exception in main: " << exception.what();
    std::cerr << std::endl;
    std::exit(EXIT_FAILURE);
  } catch (...) {
    std::cerr << "Unknown exception caught in main";
    std::cerr << std::endl;
    std::exit(EXIT_FAILURE);
  }
}