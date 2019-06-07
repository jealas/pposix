#pragma once

#include <string_view>

#include <fcntl.h>

#include "pposix/util/underlying_value.hpp"

namespace pposix::capi::file {

enum class permission : unsigned {
  none = 0u,

  user_read = S_IRUSR,
  user_write = S_IWUSR,
  user_execute = S_IXUSR,
  user_all = S_IRWXU,

  group_read = S_IRGRP,
  group_write = S_IWGRP,
  group_execute = S_IXGRP,
  group_all = S_IRWXG,

  other_read = S_IROTH,
  other_write = S_IWOTH,
  other_execute = S_IXOTH,
  other_all = S_IRWXO,
};

constexpr permission operator|(permission lhs, permission rhs) noexcept {
  return permission{util::underlying_value(lhs) | util::underlying_value(rhs)};
}

constexpr permission &operator|=(permission &lhs, permission rhs) noexcept {
  lhs = lhs | rhs;
  return lhs;
}

namespace literals::permission_literals {

namespace detail {

constexpr permission str_to_permission(char const *const c_str, size_t len,
                                       permission (*char_to_permission)(char)) {
  permission p{};

  for (const auto c : std::string_view{c_str, len}) {
    p |= char_to_permission(c);
  }

  return p;
}

template <permission Read, permission Write, permission Execute>
constexpr permission char_to_permission(const char c) noexcept {
  switch (c) {
    case 'r':
      return Read;
    case 'w':
      return Write;
    case 'x':
      return Execute;

    default:
      break;
  }

  return permission::none;
}
}  // namespace detail

constexpr permission operator"" _user(char const *c_str, size_t len) {
  return detail::str_to_permission(
      c_str, len,
      detail::char_to_permission<permission::user_read, permission::user_write,
                                 permission::user_execute>);
}

constexpr permission operator"" _group(char const *c_str, size_t len) {
  return detail::str_to_permission(
      c_str, len,
      detail::char_to_permission<permission::group_read, permission::group_write,
                                 permission::group_execute>);
}

constexpr permission operator"" _other(char const *c_str, size_t len) {
  return detail::str_to_permission(
      c_str, len,
      detail::char_to_permission<permission::other_read, permission::other_write,
                                 permission::other_execute>);
}
}  // namespace literals::permission_literals

}  // namespace pposix::capi::file
