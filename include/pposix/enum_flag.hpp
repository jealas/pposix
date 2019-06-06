#ifndef PPOSIX_ENUM_FLAG_HPP
#define PPOSIX_ENUM_FLAG_HPP

namespace pposix {

template <class Enum, Enum Value>
struct enum_flag {
  constexpr bool operator==(Enum e) const noexcept { return e == Value; }
};

}  // namespace pposix

#endif  // PPOSIX_ENUM_FLAG_HPP
