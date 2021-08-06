#include "pt.hpp"

PT_SUITE(hmm) {
  PT_TEST(test_failing_assert) { PT_ASSERT(20 == 2); }

  PT_TEST(test_empty_body) {}

  namespace outer_namespace {

  PT_TEST(test_in_named_namespace) {}

  }  // namespace outer_namespace

  PT_SPAWN(test_spawn_test) {}

  template <class Iterable>
  struct subtest_runner {
    pt::Location location;
    char const *var_name{};
    Iterable iterable;

    template <class Fn>
    auto operator-(const Fn fn) const noexcept(false) {
      for (const auto &val : iterable) {
        try {
          fn(val);
        } catch (pt::test_failed &fail) {
          fail.push_sub_fail(pt::SubFail{location, var_name, std::to_string(val)});
          throw;
        } catch (const pt::test_skipped &) {
          continue;
        }
      }
    }
  };

  struct {
    template <class Iterable>
    constexpr auto operator()(const ::pt::Location &location, const char *name,
                              Iterable iterable) const noexcept(false) {
      return subtest_runner<Iterable>{location, name, std::move(iterable)};
    }

    template <class T>
    constexpr auto operator()(const ::pt::Location &location, char const *name,
                              std::initializer_list<T> iterable) const noexcept(false) {
      return subtest_runner<std::initializer_list<T>>{location, name, iterable};
    }

  } subtest;

#define PT_LOCATION          \
  ::pt::Location {           \
    {__FILE__}, { __LINE__ } \
  }
#define PT_BETA_SUBTEST(var, ...) \
  subtest(PT_LOCATION, #var, __VA_ARGS__) - [&](const auto &var) noexcept(false)

  PT_TEST(subtests) {
    PT_BETA_SUBTEST(var, {1, 2, 3}) {
      PT_ASSERT(var < 4);

      PT_ASSERT(var < 3);
    };
  }

  PT_TEST(subtest_in_subtest) {
    PT_BETA_SUBTEST(outer, std::string{"abc"}) {
      PT_BETA_SUBTEST(inner, std::string{"def"}) {
        PT_ASSERT(outer != 'd');
        PT_ASSERT(outer != 'e');
        PT_ASSERT(outer != 'f');

        PT_ASSERT(inner != 'a');
        PT_ASSERT(inner != 'b');
        PT_ASSERT(inner != 'f');
      };
    };
  }
}