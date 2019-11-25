#pragma once

#include <string_view>
#include <system_error>

#include <sys/stat.h>
#include <unistd.h>

#include "pposix/byte_span.hpp"
#include "pposix/fcntl.hpp"
#include "pposix/platform.hpp"
#include "pposix/result.hpp"
#include "pposix/unique_fd.hpp"
#include "pposix/util.hpp"

namespace pposix {

enum class file_seek { set = SEEK_SET, current = SEEK_CUR, end = SEEK_END };

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
  return file_permission{underlying_v(lhs) | underlying_v(rhs)};
}

constexpr file_permission &operator|=(file_permission &lhs, file_permission rhs) noexcept {
  lhs = lhs | rhs;
  return lhs;
}

namespace permission_literals {

namespace detail {

constexpr file_permission str_to_permission(char const *const c_str, size_t len,
                                            file_permission (*char_to_permission)(const char)) {
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

class file {
 public:
  file() = default;

  explicit file(raw_fd fd) noexcept;

  file(const file &) = delete;
  file(file &&) = default;

  file &operator=(const file &) = delete;
  file &operator=(file &&) = default;

  static result<raw_fd> unsafe_open(const char *path, capi::access_mode mode,
                                    capi::open_flag flags) noexcept;

  template <capi::access_mode AccessMode, capi::open_flag OpenFlags>
  result<file> open(const char *path, access_mode<AccessMode> mode,
                    open_flag<OpenFlags> flags) noexcept {
    static_assert(open_flag<OpenFlags>::has(excl) ? open_flag<OpenFlags>::has(creat) : true);

    return result_map<file>(unsafe_open(path, mode, flags),
                            [](const raw_fd &fd) { return file{fd}; });
  }

  std::error_code close() noexcept;

  result<off_t> lseek(off_t offset, file_seek wh) noexcept;

  result<ssize_t> read(byte_span buffer) noexcept;
  result<ssize_t> write(byte_cspan buffer) noexcept;

  std::error_code unsafe_fcntl(capi::fcntl_cmd cmd) const noexcept;
  std::error_code unsafe_fcntl(capi::fcntl_cmd cmd, int arg) const noexcept;
  std::error_code unsafe_fcntl(capi::fcntl_cmd cmd, void *arg) const noexcept;

 private:
  unique_fd fd_{};
};

}  // namespace pposix
