#include "pt.hpp"

PT_TEST(pposix::tests, test_failing_assert) { PT_ASSERT(20 == 2); }

PT_TEST(pposix::tests, test_empty_body) {}

namespace outer_namespace {

PT_TEST(pposix::tests, test_in_named_namespace) {}

}  // namespace outer_namespace

PT_THREAD_TEST(pposix::tests, test_threaded_test) {}

PT_SPAWN_TEST(pposix::tests, test_spawn_test) {}
