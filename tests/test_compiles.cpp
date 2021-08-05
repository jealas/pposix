#include <vector>

#include "pt.hpp"

PT_SUITE(std::vector) {
  PT_TEST(failing_assert) { PT_ASSERT(20 == 2); }

  PT_TEST(empty_body) {}

  namespace outer_namespace {

  PT_TEST(in_named_namespace) {}

  }  // namespace outer_namespace

  PT_SPAWN_TEST(spawn_test) {}

  PT_TEST(push_back_on_empty) {
    std::vector<int> v{};
    PT_ASSERT(v.size() == 0);
    v.push_back({});
    PT_ASSERT(v.size() == 1);
  }
}

namespace {

PT_SUITE("anonymous original test name") {
  PT_TEST(original_name) {}
}

namespace original_name {

PT_SUITE("nested copied test name") {
  PT_TEST(original_name) {}
}

}  // namespace original_name

namespace copied_name {

PT_SUITE("copied test name") {
  PT_TEST(original_name) {}
}

}  // namespace copied_name

}  // namespace
