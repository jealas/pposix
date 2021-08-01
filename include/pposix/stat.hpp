#pragma once

#include <sys/stat.h>

#include "pposix/util.hpp"

namespace pposix {

namespace capi {

enum class file_type : ::mode_t {
  blk = S_IFBLK,
  chr = S_IFCHR,
  fifo = S_IFIFO,
  ifreg = S_IFREG,
  iflnk = S_IFLNK,
  ifsock = S_IFSOCK
};

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
  return permission{underlying_v(lhs) | underlying_v(rhs)};
}

constexpr permission &operator|=(permission &lhs, permission rhs) noexcept {
  lhs = lhs | rhs;
  return lhs;
}

}  // namespace capi

namespace detail {

constexpr capi::permission str_to_permission(char const *const c_str, size_t len,
                                             capi::permission (*char_to_permission)(const char)) {
  capi::permission p{};

  for (const auto c : std::string_view{c_str, len}) {
    p |= char_to_permission(c);
  }

  return p;
}

template <capi::permission Read, capi::permission Write, capi::permission Execute>
constexpr capi::permission char_to_permission(const char c) noexcept {
  switch (c) {
    case 'R':
    case 'r':
      return Read;

    case 'W':
    case 'w':
      return Write;

    case 'X':
    case 'x':
      return Execute;

    default:
      break;
  }

  return capi::permission::none;
}

}  // namespace detail

constexpr capi::permission operator"" _user(char const *c_str, size_t len) {
  return detail::str_to_permission(
      c_str, len,
      detail::char_to_permission<capi::permission::user_read, capi::permission::user_write,
                                 capi::permission::user_execute>);
}

constexpr capi::permission operator"" _group(char const *c_str, size_t len) {
  return detail::str_to_permission(
      c_str, len,
      detail::char_to_permission<capi::permission::group_read, capi::permission::group_write,
                                 capi::permission::group_execute>);
}

constexpr capi::permission operator"" _other(char const *c_str, size_t len) {
  return detail::str_to_permission(
      c_str, len,
      detail::char_to_permission<capi::permission::other_read, capi::permission::other_write,
                                 capi::permission::other_execute>);
}

}  // namespace pposix
