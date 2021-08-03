#include "pt.hpp"

// PT_TEST(pposix::tests, test_hello_world1) {
//   for (auto i{0}; i <= 100; ++i, ++i, ++i) {
//     PT_SUBTEST(pt::range(1, 20), [](const auto e) { PT_ASSERT(e < 20); });

//    std::vector<int> v{1, 2, 3, 4, 5, 666};
//    PT_SUBTEST(e, pt::range(v)) {
//      std::cout << *e << std::endl;
//      PT_ASSERT(*e <= 100);
//    }
//  }
//}

PT_TEST(pposix::tests, test_hello_world2) { PT_ASSERT(20 == 2); }

PT_TEST(pposix::tests, test_hello_world3) {}
