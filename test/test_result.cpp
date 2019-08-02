#include <catch2/catch.hpp>

#include <system_error>

#include "pposix/result.hpp"

#define PPOSIX_RESULT_TYPES int

TEMPLATE_TEST_CASE("Results can be constructed", "[pposix][result]", PPOSIX_RESULT_TYPES) {
  WHEN("constructing with a std::error_code") {
    const auto errc =
        GENERATE(std::errc{}, std::errc::io_error, std::errc::resource_unavailable_try_again,
                 std::errc::value_too_large);

    pposix::result<TestType> res{std::make_error_code(errc)};

    THEN("there is no value") { REQUIRE(not res.has_value()); }
    AND_THEN("there is an error") {
      REQUIRE(res.has_error());
      REQUIRE(res.error() == std::make_error_code(errc));
    }
    AND_THEN("the boolean value of the resource is false") { REQUIRE(not res); }
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
}
