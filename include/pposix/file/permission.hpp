#pragma once

#include <string_view>

#include <fcntl.h>

#include "pposix/file/permission.hpp"
#include "pposix/util.hpp"

namespace pposix::file {

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
  return permission{underlying_value(lhs) | underlying_value(rhs)};
}

constexpr permission &operator|=(permission &lhs, permission rhs) noexcept {
  lhs = lhs | rhs;
  return lhs;
}

}  // namespace pposix::file

namespace pposix::file::literals::permission_literals {

namespace detail {

constexpr file::permission str_to_permission(char const *const c_str, size_t len,
                                             file::permission (*char_to_permission)(char)) {
  file::permission p{};

  for (const auto c : std::string_view{c_str, len}) {
    p |= char_to_permission(c);
  }

  return p;
}

template <file::permission Read, file::permission Write, file::permission Execute>
constexpr file::permission char_to_permission(const char c) noexcept {
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

  return file::permission::none;
}
}  // namespace detail

constexpr file::permission operator"" _user(char const *c_str, size_t len) {
  return detail::str_to_permission(
      c_str, len,
      detail::char_to_permission<file::permission::user_read, file::permission::user_write,
                                 file::permission::user_execute>);
}

constexpr file::permission operator"" _group(char const *c_str, size_t len) {
  return detail::str_to_permission(
      c_str, len,
      detail::char_to_permission<file::permission::group_read, file::permission::group_write,
                                 file::permission::group_execute>);
}

constexpr file::permission operator"" _other(char const *c_str, size_t len) {
  return detail::str_to_permission(
      c_str, len,
      detail::char_to_permission<file::permission::other_read, file::permission::other_write,
                                 file::permission::other_execute>);
}

}  // namespace pposix::file::literals::permission_literals
