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
struct exclusive_enum_flag {
  using enum_t = std::decay_t<Enum>;
  static_assert(std::is_enum_v<enum_t>);

  constexpr explicit operator Enum() const noexcept { return Value; }
};

template <class Enum, Enum Value>
struct enum_flag {
  using enum_t = std::decay_t<Enum>;
  static_assert(std::is_enum_v<enum_t>);

  constexpr explicit operator Enum() const noexcept { return Value; }
};

template <class Enum, Enum Flags>
class enum_flag_set {
  using enum_t = std::decay_t<Enum>;
  static_assert(std::is_enum_v<enum_t>);

 public:
  static constexpr bool has(Enum value) noexcept {
    return underlying_value(Flags) & underlying_value(value);
  }

  template <Enum Constant>
  static constexpr bool has(enum_flag<Enum, Constant>) noexcept {
    return underlying_value(Flags) & underlying_value(Constant);
  }
};

template <class Enum, Enum Lhs, Enum Rhs>
constexpr enum_flag_set<Enum, Enum{underlying_value(Lhs) | underlying_value(Rhs)}> operator|(
    enum_flag<Enum, Lhs>, enum_flag<Enum, Rhs>) noexcept {
  using enum_t = std::decay_t<Enum>;
  static_assert(std::is_enum_v<enum_t>);

  return {};
}

template <class Enum, Enum LhsFlags, Enum RhsFlag>
constexpr enum_flag_set<Enum, Enum{underlying_value(LhsFlags) | underlying_value(RhsFlag)}>
operator|(enum_flag_set<Enum, LhsFlags>, enum_flag<Enum, RhsFlag>) noexcept {
  using enum_t = std::decay_t<Enum>;
  static_assert(std::is_enum_v<enum_t>);

  return {};
}

template <class T>
constexpr bool always_false = false;

}  // namespace pposix
