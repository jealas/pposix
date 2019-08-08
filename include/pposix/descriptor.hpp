#pragma once

#include <type_traits>

namespace pposix {

struct null_descriptor_t {};
inline constexpr null_descriptor_t null_descriptor{};

template <class T, T Null>
class descriptor {
 public:
  constexpr descriptor() noexcept = default;
  constexpr descriptor(T value) noexcept : value_{value} {}
  constexpr descriptor(null_descriptor_t) noexcept : descriptor{} {}

  constexpr descriptor(const descriptor&) noexcept = default;
  constexpr descriptor(descriptor&&) noexcept = default;

  constexpr descriptor& operator=(const descriptor&) noexcept = default;
  constexpr descriptor& operator=(descriptor&&) noexcept = default;

  constexpr descriptor& operator=(null_descriptor_t) noexcept {
    value_ = Null;
    return *this;
  }

  constexpr T raw() noexcept { return value_; }
  constexpr const T raw() const noexcept { return value_; }

 private:
  T value_{Null};
};

template <class T, T Null>
constexpr bool operator==(descriptor<T, Null> lhs, descriptor<T, Null> rhs) noexcept {
  return lhs.raw() == rhs.raw();
}

template <class T, T Null>
constexpr bool operator==(descriptor<T, Null> lhs, null_descriptor_t) noexcept {
  return lhs.raw() == Null;
}

template <class>
struct is_descriptor : std::false_type {};

template <class T, T Null>
struct is_descriptor<descriptor<T, Null>> : std::true_type {};

template <class T>
inline constexpr bool is_descriptor_v = is_descriptor<T>::value;

}  // namespace pposix
