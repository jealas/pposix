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

template <class Enum, Enum Value>
struct exclusive_enum_flag {
  using enum_t = std::decay_t<Enum>;
  static_assert(std::is_enum_v<enum_t>);

  constexpr operator Enum() const noexcept { return Value; } // NOLINT implicit conversion
};

template <class Enum, Enum Flags>
class enum_flag {
  using enum_t = std::decay_t<Enum>;
  static_assert(std::is_enum_v<enum_t>);

 public:
  constexpr operator Enum() const noexcept { return Flags; } // NOLINT implicit conversion

  static constexpr bool has(Enum value) noexcept {
    return underlying_v(Flags) & underlying_v(value);
  }

  template <Enum Constant>
  static constexpr bool has(enum_flag<Enum, Constant>) noexcept {
    return underlying_v(Flags) & underlying_v(Constant);
  }
};

template <class Enum, Enum Lhs, Enum Rhs>
constexpr enum_flag<Enum, Enum{underlying_v(Lhs) | underlying_v(Rhs)}> operator|(
    enum_flag<Enum, Lhs>, enum_flag<Enum, Rhs>) noexcept {
  using enum_t = std::decay_t<Enum>;
  static_assert(std::is_enum_v<enum_t>);

  return {};
}

template <class>
constexpr bool always_false = false;

}  // namespace pposix

#define PPOSIX_COMMON_CALL(fn, ...) \
  ((fn)(__VA_ARGS__) == -1 ? ::pposix::current_errno_code() : ::std::error_code{})

#define PPOSIX_COMMON_RESULT_CALL_IMPL(fn, ...)                               \
  if (const auto _pposix_result_{fn(__VA_ARGS__)}; _pposix_result_ == -1) { \
    return ::pposix::current_errno_code();                                    \
  } else {                                                                    \
    return _pposix_result_;                                                   \
  }

#define PPOSIX_COMMON_RESULT_MAP_IMPL(T, fn, ...)                             \
  if (const auto _pposix_result_{fn(__VA_ARGS__)}; _pposix_result_ == -1) { \
    return ::pposix::current_errno_code();                                    \
  } else {                                                                    \
    return T{_pposix_result_};                                                \
  }
