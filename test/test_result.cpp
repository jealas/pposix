#include <catch2/catch.hpp>

#include <system_error>

#include "pposix/result.hpp"

#define PPOSIX_RESULT_TYPES char, int, long long, float, double, long double

TEMPLATE_TEST_CASE("Results can be constructed", "[pposix][result]", PPOSIX_RESULT_TYPES) {
  GIVEN("a result constructed with a std::error_code") {
    const auto errc =
        GENERATE(std::errc{}, std::errc::io_error, std::errc::resource_unavailable_try_again,
                 std::errc::value_too_large);

    pposix::result<TestType> res{std::make_error_code(errc)};

    THEN("it has no value") { REQUIRE(not res.has_value()); }
    AND_THEN("it has an error") {
      REQUIRE(res.has_error());
      REQUIRE(res.error() == std::make_error_code(errc));
    }
    AND_THEN("the boolean value of the resource is false") { REQUIRE(not res); }
    AND_THEN("trying to get the value throws a bad_result_access exception") {
      REQUIRE_THROWS_AS(res.value(), pposix::bad_result_access);
    }
    AND_THEN("mapping the result to a new type of result doesn't invoke the given functor") {
      const auto new_res{pposix::result_map<bool>(res, [](TestType) {
        // This functor should never execute since the result has an error in it.
        REQUIRE(false);
        return false;
      })};
      AND_THEN("the error code is preserved in the new result type") {
        REQUIRE(new_res.error() == std::make_error_code(errc));
        REQUIRE(new_res.error() == res.error());
      }
    }
  }

  GIVEN("a result constructed with a value type") {
    const auto value = GENERATE(TestType{}, TestType{} + 1, TestType{} + 42);

    pposix::result<TestType> res{value};

    THEN("it has a value") {
      REQUIRE(res.has_value());
      REQUIRE(*res == value);
      REQUIRE(res.value() == value);
    }
    AND_THEN("it doesn't have an error") { REQUIRE(not res.has_error()); }
    AND_THEN("the boolean value of the resource is true") { REQUIRE(res); }
    AND_THEN("trying to get the error throws a bad_result_access exception") {
      REQUIRE_THROWS_AS(res.error(), pposix::bad_result_access);
    }
    AND_THEN(
        "mapping the result to a new type of result does invoke the given functor with the value "
        "held in the result") {
      bool invoked{false};

      pposix::result_map<bool>(res, [&](TestType val) {
        REQUIRE(val == value);
        invoked = true;
        return true;
      });

      REQUIRE(invoked);
    }
    AND_THEN("the new result holds the transformed value") {
      const auto new_res{pposix::result_map<TestType>(res, [](TestType val) { return val + 1; })};

      REQUIRE(not new_res.has_error());
      REQUIRE(new_res.has_value());
      REQUIRE(new_res);
      REQUIRE(new_res.value() == value + 1);
      REQUIRE(*new_res == value + 1);
    }
  }
}
