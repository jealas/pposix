#pragma once

#include <type_traits>

namespace pposix {

template <class Enum>
constexpr decltype(auto) underlying_value(const Enum e) {
  using enum_t = std::decay_t<Enum>;

  static_assert(std::is_enum_v<enum_t>);

  return static_cast<std::underlying_type_t<enum_t>>(e);
}

template <class T>
constexpr bool always_false = false;

}  // namespace pposix
