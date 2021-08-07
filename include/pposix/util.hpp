#pragma once

#include <system_error>
#include <type_traits>

#include "pposix/errno.hpp"

namespace pposix {

template <class Enum>
constexpr decltype(auto) underlying_v(const Enum e) {
  using enum_t = std::decay_t<Enum>;
  static_assert(std::is_enum_v<enum_t>);

  return static_cast<std::underlying_type_t<enum_t>>(e);
}

template <class>
constexpr bool always_false = false;

}  // namespace pposix

#define PPOSIX_COMMON_CALL(fn, ...) \
  ((fn)(__VA_ARGS__) == -1 ? ::pposix::current_errno_code() : ::std::error_code{})

#define PPOSIX_COMMON_RESULT_CALL_IMPL(fn, ...)                             \
  if (const auto _pposix_result_{fn(__VA_ARGS__)}; _pposix_result_ == -1) { \
    return ::pposix::current_errno_code();                                  \
  } else {                                                                  \
    return _pposix_result_;                                                 \
  }

#define PPOSIX_COMMON_RESULT_DOUBLE_WRAP_IMPL(outer, inner, fn, ...)        \
  if (const auto _pposix_result_{fn(__VA_ARGS__)}; _pposix_result_ == -1) { \
    return ::pposix::current_errno_code();                                  \
  } else {                                                                  \
    return outer{inner{_pposix_result_}};                                   \
  }

#define PPOSIX_COMMON_RESULT_MAP_IMPL(T, fn, ...)                           \
  if (const auto _pposix_result_{fn(__VA_ARGS__)}; _pposix_result_ == -1) { \
    return ::pposix::current_errno_code();                                  \
  } else {                                                                  \
    return T{_pposix_result_};                                              \
  }
