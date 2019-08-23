#pragma once

#include <type_traits>

namespace pposix {

struct null_descriptor_t {};
inline constexpr null_descriptor_t null_descriptor{};

template <class Tag, class T, class GetNull>
class descriptor {
 public:
  constexpr descriptor() noexcept : value_{GetNull{}()} {}
  constexpr descriptor(T value) noexcept : value_{value} {}
  constexpr descriptor(null_descriptor_t) noexcept : descriptor{} {}

  constexpr descriptor(const descriptor&) noexcept = default;
  constexpr descriptor(descriptor&&) noexcept = default;

  constexpr descriptor& operator=(const descriptor&) noexcept = default;
  constexpr descriptor& operator=(descriptor&&) noexcept = default;

  constexpr descriptor& operator=(null_descriptor_t) noexcept {
    value_ = GetNull{}();
    return *this;
  }

  constexpr T raw() noexcept { return value_; }
  constexpr const T raw() const noexcept { return value_; }

 private:
  T value_{};
};

template <class Tag, class T, class GetNull>
constexpr bool operator==(descriptor<Tag, T, GetNull> lhs,
                          descriptor<Tag, T, GetNull> rhs) noexcept {
  return lhs.raw() == rhs.raw();
}

template <class Tag, class T, class GetNull>
constexpr bool operator==(descriptor<Tag, T, GetNull> lhs, null_descriptor_t) noexcept {
  return lhs.raw() == GetNull{}();
}

template <class Tag, class T, class GetNull>
constexpr bool operator!=(descriptor<Tag, T, GetNull> lhs,
                          descriptor<Tag, T, GetNull> rhs) noexcept {
  return !(lhs == rhs);
}

template <class Tag, class T, class GetNull>
constexpr bool operator!=(descriptor<Tag, T, GetNull> lhs, null_descriptor_t) noexcept {
  return !(lhs == null_descriptor);
}

template <class>
struct is_descriptor : std::false_type {};

template <class Tag, class T, class GetNull>
struct is_descriptor<descriptor<Tag, T, GetNull>> : std::true_type {};

template <class T>
inline constexpr bool is_descriptor_v = is_descriptor<T>::value;

}  // namespace pposix
