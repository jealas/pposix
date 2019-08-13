#include "pposix_test.hpp"

#include <initializer_list>
#include <system_error>

#include "pposix/result.hpp"

#define PPOSIX_RESULT_TYPES char, int, long long, float, double, long double

test_case_template("Results can be constructed", TestType, PPOSIX_RESULT_TYPES) {
  given("a result constructed with a std::error_code") {
    std::errc errc;
    const std::initializer_list<std::errc> error_codes{std::errc{}, std::errc::io_error,
                                                       std::errc::resource_unavailable_try_again,
                                                       std::errc::value_too_large};

    generate(errc, error_codes);

    pposix::result<TestType> res{std::make_error_code(errc)};

    then("it has no value") { require(not res.has_value()); }
    and_then("it has an error") {
      require(res.has_error());
      require(res.error() == std::make_error_code(errc));
    }
    and_then("the boolean value of the resource is false") { require(not res); }
    and_then("trying to get the value throws a bad_result_access exception") {
      require_throws_as((void)res.value(), pposix::bad_result_access);
    }
    and_then("mapping the result to a new type of result doesn't invoke the given functor") {
      const auto new_res{pposix::result_map<bool>(res, [](TestType) {
        // This functor should never execute since the result has an error in it.
        require(false);
        return false;
      })};
      and_then("the error code is preserved in the new result type") {
        require(new_res.error() == std::make_error_code(errc));
        require(new_res.error() == res.error());
      }
    }
  }

  given("a result constructed with a value type") {
    TestType value;
    const std::initializer_list<TestType> values{TestType{}, TestType{} + 1, TestType{} + 42};
    generate(value, values);

    pposix::result<TestType> res{value};

    then("it has a value") {
      require(res.has_value());
      require(*res == value);
      require(res.value() == value);
    }
    and_then("it doesn't have an error") { require(not res.has_error()); }
    and_then("the boolean value of the resource is true") { require(res); }
    and_then("trying to get the error throws a bad_result_access exception") {
      require_throws_as((void)res.error(), pposix::bad_result_access);
    }
    and_then(
        "mapping the result to a new type of result does invoke the given functor with the value "
        "held in the result") {
      bool invoked{false};

      pposix::result_map<bool>(res, [&](TestType val) {
        require(val == value);
        invoked = true;
        return true;
      });

      require(invoked);
    }
    and_then("the new result holds the transformed value") {
      const auto new_res{pposix::result_map<TestType>(res, [](TestType val) { return val + 1; })};

      require(not new_res.has_error());
      require(new_res.has_value());
      require(new_res);
      require(new_res.value() == value + 1);
      require(*new_res == value + 1);
    }
  }
}
