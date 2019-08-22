#pragma once

#include <string_view>
#include <system_error>

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "pposix/byte_span.hpp"
#include "pposix/platform.hpp"
#include "pposix/result.hpp"
#include "pposix/unique_fd.hpp"
#include "pposix/util.hpp"

namespace pposix {

// File flags
enum class file_flags : unsigned {
  append = O_APPEND,
  cloexec = O_CLOEXEC,
  create = O_CREAT,

  directory = O_DIRECTORY,

#if !PPOSIX_PLATFORM_FREEBSD
  dsync = O_DSYNC,
#endif

  exclusive = O_EXCL,
  noctty = O_NOCTTY,
  nofollow = O_NOFOLLOW,
  nonblock = O_NONBLOCK,

#if !PPOSIX_PLATFORM_MACOS && !PPOSIX_PLATFORM_FREEBSD
  rsync = O_RSYNC,
#endif

  sync = O_SYNC,
  truncate = O_TRUNC,

#if !PPOSIX_PLATFORM_LINUX && !PPOSIX_PLATFORM_OPENBSD
  tty_init = O_TTY_INIT
#endif
};

constexpr file_flags operator|(file_flags lhs, file_flags rhs) noexcept {
  return file_flags{underlying_value(lhs) | underlying_value(rhs)};
}

constexpr file_flags &operator|=(file_flags &lhs, file_flags rhs) noexcept {
  lhs = lhs | rhs;
  return lhs;
}

// File mode
enum class file_mode : unsigned {
#if !PPOSIX_PLATFORM_LINUX && !PPOSIX_PLATFORM_OPENBSD
  exec = O_EXEC,
#endif

  read = O_RDONLY,
  read_write = O_RDWR,

#if !PPOSIX_PLATFORM_LINUX && !PPOSIX_PLATFORM_FREEBSD && !PPOSIX_PLATFORM_OPENBSD
  search = O_SEARCH,
#endif

  write = O_WRONLY
};

// File seek whence
enum class file_seek { set = SEEK_SET, current = SEEK_CUR, end = SEEK_END };

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

// File open: file
result<unique_fd<raw_fd>> open(const char *path, file_mode mode, file_flags flags) noexcept;

// File close
std::error_code close(raw_fd fd) noexcept;

// File dup
result<unique_fd<raw_fd>> dup(raw_fd fd) noexcept;

// File lseek
result<off_t> lseek(raw_fd fd, off_t offset, file_seek wh) noexcept;

// File read
result<ssize_t> read(raw_fd fd, byte_span buffer) noexcept;

// File write
result<ssize_t> write(raw_fd fd, byte_cspan buffer) noexcept;

}  // namespace pposix
