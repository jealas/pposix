#pragma once

#include <type_traits>

namespace pposix {

template <class Enum, Enum Value>
struct enum_flag {
  static_assert(std::is_enum_v<Enum>);

  constexpr bool operator==(Enum e) const noexcept { return e == Value; }

  template <Enum OtherValue>
  constexpr bool operator==(enum_flag<Enum, OtherValue>) const noexcept {
    return Value == OtherValue;
  }

  template <Enum OtherValue>
  constexpr enum_flag<Enum, Value | OtherValue> operator|(enum_flag<Enum, OtherValue>) const
      noexcept {
    return {};
  }
};

}  // namespace pposix
