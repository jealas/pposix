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

// File type
template <capi::file_type FileType>
using file_type = enum_flag<capi::file_type, FileType>;

constexpr file_type<capi::file_type::blk> blk{};
constexpr file_type<capi::file_type::chr> chr{};
constexpr file_type<capi::file_type::fifo> fifo{};
constexpr file_type<capi::file_type::ifreg> ifreg{};
constexpr file_type<capi::file_type::iflnk> iflnk{};
constexpr file_type<capi::file_type::ifsock> ifsock{};

// Permissions
template <capi::permission Permission>
using permission = enum_flag<capi::permission, Permission>;

constexpr permission<capi::permission::none> no_permissions{};

constexpr permission<capi::permission::user_read> user_read{};
constexpr permission<capi::permission::user_write> user_write{};
constexpr permission<capi::permission::user_execute> user_execute{};
constexpr permission<capi::permission::user_all> user_all{};

constexpr permission<capi::permission::group_read> group_read{};
constexpr permission<capi::permission::group_write> group_write{};
constexpr permission<capi::permission::group_execute> group_execute{};
constexpr permission<capi::permission::group_all> group_all{};

constexpr permission<capi::permission::other_read> other_read{};
constexpr permission<capi::permission::other_write> other_write{};
constexpr permission<capi::permission::other_execute> other_execute{};
constexpr permission<capi::permission::other_all> other_all{};

}  // namespace pposix
