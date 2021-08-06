#include "pt.hpp"

PT_SUITE(hmm) {
  PT_TEST(test_failing_assert) { PT_ASSERT(20 == 2); }

  PT_TEST(test_empty_body) {}

  namespace outer_namespace {

  PT_TEST(test_in_named_namespace) {}

  }  // namespace outer_namespace

  PT_SPAWN(test_spawn_test) {}

  PT_TEST(subtests) {
    PT_SUBTEST(var, {1, 2, 3}) { PT_ASSERT(var < 4); };
  }

  PT_TEST(subtest_in_subtest) {
    PT_SUBTEST(outer, std::string{"abc"}) {
      PT_ASSERT(outer != 'd');
      PT_ASSERT(outer != 'e');
      PT_ASSERT(outer != 'f');

      PT_SUBTEST(inner, std::string{"def"}) {
        PT_ASSERT(outer != 'd');
        PT_ASSERT(outer != 'e');
        PT_ASSERT(outer != 'f');

        PT_ASSERT(inner != 'a');
        PT_ASSERT(inner != 'b');
        PT_ASSERT(inner != 'c');
      };
    };
  }

  PT_TEST(subtests_in_section) {
    PT_SECTION("This is a section name") {
      PT_SUBTEST(v, {1, 2, 3}) { PT_ASSERT(v < 3); };
    };
  }
}