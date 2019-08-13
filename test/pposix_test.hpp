#pragma once

#define DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES

#include <doctest.h>

#include <algorithm>
#include <string>
#include <vector>

#define test_suite DOCTEST_TEST_SUITE
#define test_case DOCTEST_TEST_CASE
#define test_case_template DOCTEST_TEST_CASE_TEMPLATE
#define subcase DOCTEST_SUBCASE

#define scenario DOCTEST_SCENARIO
#define scenario_template DOCTEST_SCENARIO_TEMPLATE
#define given DOCTEST_GIVEN
#define when DOCTEST_WHEN
#define then DOCTEST_THEN
#define and_then DOCTEST_AND_THEN
#define and_when DOCTEST_AND_WHEN

#define require DOCTEST_REQUIRE_UNARY
#define require_not DOCTEST_REQUIRE_UNARY_FALSE
#define require_eq DOCTEST_REQUIRE_EQ
#define require_ne DOCTEST_REQUIRE_NE
#define require_gt DOCTEST_REQUIRE_GT
#define require_lt DOCTEST_REQUIRE_LT

#define require_throws DOCTEST_REQUIRE_THROWS
#define require_throws_as DOCTEST_REQUIRE_THROWS_AS
#define require_nothrow DOCTEST_REQUIRE_NOTHROW

#define check DOCTEST_CHECK_UNARY
#define check_not DOCTEST_CHECK_UNARY_FALSE
#define check_eq DOCTEST_CHECK_EQ
#define check_ne DOCTEST_CHEC_NE
#define check_gt DOCTEST_CHECK_GT
#define check_lt DOCTEST_CHECK_LT

#define check_throws DOCTEST_CHECK_THROWS
#define check_throws_as DOCTEST_CHECK_THROWS_AS
#define check_nothrow DOCTEST_CHECK_NOTHROW

#define DOCTEST_GENERATE(data, data_array)                                                \
  static std::vector<std::string> _doctest_subcases = [&data_array]() {                   \
    std::vector<std::string> out;                                                         \
    while (out.size() != data_array.size())                                               \
      out.push_back(std::string(#data_array "[") + std::to_string(out.size() + 1) + "]"); \
    return out;                                                                           \
  }();                                                                                    \
  int _doctest_subcase_idx = 0;                                                           \
  std::for_each(data_array.begin(), data_array.end(), [&](const auto& in) {               \
    DOCTEST_SUBCASE(_doctest_subcases[_doctest_subcase_idx++].c_str()) { data = in; }     \
  })

#define generate DOCTEST_GENERATE