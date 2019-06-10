#pragma once

namespace pposix {

template <class Enum, Enum Value>
struct enum_flag {
  constexpr bool operator==(Enum e) const noexcept { return e == Value; }
};

}  // namespace pposix
