#include "pt.hpp"

PT_SUITE(hmm) {
  PT_TEST(test_failing_assert) { PT_ASSERT(20 == 2); }

  PT_TEST(test_empty_body) {}

  namespace outer_namespace {

  PT_TEST(test_in_named_namespace) {}

  }  // namespace outer_namespace

  PT_THREAD_TEST(test_threaded_test) {}

  PT_SPAWN_TEST(test_spawn_test) {}

}  // namespace
