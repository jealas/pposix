#pragma once

#include <type_traits>

namespace pposix {

template <class Enum>
constexpr decltype(auto) underlying_value(const Enum e) {
  using enum_t = std::decay_t<Enum>;

  static_assert(std::is_enum_v<enum_t>);

  return static_cast<std::underlying_type_t<enum_t>>(e);
}

template <class Enum, Enum Value>
struct enum_constant {
  constexpr operator Enum() const noexcept { return Value; }
};

template <class Enum, Enum Lhs, Enum Rhs>
constexpr enum_constant<Enum, Enum{underlying_value(Lhs) | underlying_value(Rhs)}> operator|(
    enum_constant<Enum, Lhs>, enum_constant<Enum, Rhs>) noexcept {
  return {};
}

template <class T>
constexpr bool always_false = false;

}  // namespace pposix
