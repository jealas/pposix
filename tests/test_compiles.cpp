#include <list>
#include <vector>

#include "pt.hpp"

PT_SUITE(std::vector) {
  PT_TEST(failing_assert) { PT_ASSERT(20 == 2); }

  PT_TEST(empty_body) {}

  namespace outer_namespace {

  PT_TEST(in_named_namespace) {}

  }  // namespace outer_namespace

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

namespace throws_macro {

PT_SUITE(PT_THROWS) {
  PT_TEST(no_throw) { PT_NOTHROWS(3); }

  PT_TEST(assert_inside_no_throw) { PT_NOTHROWS(PT_ASSERT(false)); }

  PT_TEST(throws) {
    PT_THROWS(std::exception, []() { throw std::exception{}; }());
    PT_THROWS(const char *, []() { throw ""; }());
    PT_THROWS(int, []() { throw 1; }());

    struct {
    } anonymous_struct{};

    pt::throws<decltype(anonymous_struct)>({PT_ASSERTION_LINE}) = []() {
      struct {
      } anonymous{};
      throw anonymous;
    };

    pt::throws<decltype(anonymous_struct)>() = []() {
      struct {
      } anonymous{};
      throw anonymous;
    };
  }

  PT_TEST(manual_subtest) {
    pt::subtest({1, 2, 3}) = [](int) {};
    pt::subtest(std::vector<int>{4, 5, 6}) = [](int) {};
    pt::subtest(PT_LOCATION, "", "characters") = [](int) {};
    pt::subtest(PT_LOCATION, nullptr, std::list<int>{4, 5, 6}) = [](int) {};
  }

  PT_TEST(manual_section) {
    pt::section(nullptr) = []() {};
    pt::section("") = []() {};
    pt::section("manual section") = []() {};
    pt::section(PT_LOCATION, "manual section with location") = []() {};
  }

  PT_TEST(subtest_mutation) {
    std::vector<int> v{};

    PT_SUBTEST(e, v) { v.push_back(e); };
  }
}

}  // namespace throws_macro