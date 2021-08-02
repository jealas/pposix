#include "pposix_test.hpp"

PPOSIX_TEST(pposix::tests, test_hello_world1, []() { PPOSIX_ASSERT(20 == 4); })

PPOSIX_TEST(pposix::tests, test_hello_world2, []() { PPOSIX_ASSERT(20 == 2); })

PPOSIX_TEST(pposix::tests, test_hello_world3, []() {})
