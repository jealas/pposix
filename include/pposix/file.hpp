#pragma once

#include <string_view>
#include <system_error>

#include <fcntl.h>
#include <unistd.h>

#include "pposix/byte_span.hpp"
#include "pposix/result.hpp"
#include "pposix/unique_fd.hpp"
#include "pposix/util.hpp"

namespace pposix {

// File flags
enum class file_flags : unsigned {
  append = O_APPEND,
  create = O_CREAT,
  dsync = O_DSYNC,
  exclusive = O_EXCL,
  noctty = O_NOCTTY,
  nonblock = O_NONBLOCK,
  rsync = O_RSYNC,
  sync = O_SYNC,
  truncate = O_TRUNC,
};

constexpr file_flags operator|(file_flags lhs, file_flags rhs) noexcept {
  return file_flags{underlying_value(lhs) | underlying_value(rhs)};
}

constexpr file_flags &operator|=(file_flags &lhs, file_flags rhs) noexcept {
  lhs = lhs | rhs;
  return lhs;
}

// File whence
enum class file_whence { set = SEEK_SET, current = SEEK_CUR, end = SEEK_END };

// File mode
enum class file_mode : unsigned { read = O_RDONLY, write = O_WRONLY, read_write = O_RDWR };

// File permission

enum class file_permission : unsigned {
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

constexpr file_permission operator|(file_permission lhs, file_permission rhs) noexcept {
  return file_permission{underlying_value(lhs) | underlying_value(rhs)};
}

constexpr file_permission &operator|=(file_permission &lhs, file_permission rhs) noexcept {
  lhs = lhs | rhs;
  return lhs;
}

namespace permission_literals {

namespace detail {

constexpr file_permission str_to_permission(char const *const c_str, size_t len,
                                            file_permission (*char_to_permission)(char)) {
  file_permission p{};

  for (const auto c : std::string_view{c_str, len}) {
    p |= char_to_permission(c);
  }

  return p;
}

template <file_permission Read, file_permission Write, file_permission Execute>
constexpr file_permission char_to_permission(const char c) noexcept {
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

  return file_permission::none;
}

}  // namespace detail

constexpr file_permission operator"" _user(char const *c_str, size_t len) {
  return detail::str_to_permission(
      c_str, len,
      detail::char_to_permission<file_permission::user_read, file_permission::user_write,
                                 file_permission::user_execute>);
}

constexpr file_permission operator"" _group(char const *c_str, size_t len) {
  return detail::str_to_permission(
      c_str, len,
      detail::char_to_permission<file_permission::group_read, file_permission::group_write,
                                 file_permission::group_execute>);
}

constexpr file_permission operator"" _other(char const *c_str, size_t len) {
  return detail::str_to_permission(
      c_str, len,
      detail::char_to_permission<file_permission::other_read, file_permission::other_write,
                                 file_permission::other_execute>);
}

}  // namespace permission_literals

// File close
std::error_code close(const raw_fd &fd) noexcept;

// File dup
result<unique_fd<raw_fd>> dup(const raw_fd &fd) noexcept;

// File lseek
result<off_t> lseek(const raw_fd &fd, off_t offset, file_whence wh) noexcept;

// File read
result<ssize_t> read(const raw_fd &fd, byte_span buffer) noexcept;

// File write
result<ssize_t> write(const raw_fd &fd, byte_cspan buffer) noexcept;

}  // namespace pposix