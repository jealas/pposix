#pragma once

#include <sys/stat.h>
#include <unistd.h>

#include <string_view>
#include <system_error>

#include "pposix/byte_span.hpp"
#include "pposix/fcntl.hpp"
#include "pposix/file_descriptor.hpp"
#include "pposix/platform.hpp"
#include "pposix/result.hpp"
#include "pposix/stat.hpp"
#include "pposix/util.hpp"

namespace pposix {

enum class file_seek { set = SEEK_SET, current = SEEK_CUR, end = SEEK_END };

class file {
 public:
  file() = default;

  explicit file(fd fd) noexcept;

  file(const file &) = delete;
  file(file &&) = default;

  file &operator=(const file &) = delete;
  file &operator=(file &&) = default;

  template <capi::access_mode AccessMode, capi::open_flag OpenFlags>
  result<file> open(const char *path, access_mode<AccessMode> access,
                    open_flag<OpenFlags> flags) noexcept {
    static_assert(open_flag<OpenFlags>::has(excl) ? open_flag<OpenFlags>::has(creat) : true,
                  "Specifying 'excl' without 'creat' is undefined. Add '| creat' to your open "
                  "flags to correct.");

    static_assert(not open_flag<OpenFlags>::has(creat),
                  "You must provide the 'permission' argument when specifying 'creat'. Use the "
                  "open(path, access_mode, open_flag, permission) overload instead.");

    return result_map<file>(capi::open(path, access, flags),
                            [](fd descriptor) { return file{std::move(descriptor)}; });
  }

  template <capi::access_mode AccessMode, capi::open_flag OpenFlags, capi::permission Permission>
  result<file> open(const char *path, const access_mode<AccessMode> access,
                    const open_flag<OpenFlags> flags, const permission<Permission> perm) noexcept {
    static_assert(open_flag<OpenFlags>::has(excl) ? open_flag<OpenFlags>::has(creat) : true,
                  "Specifying 'excl' without 'creat' is undefined. Add '| creat' to your open "
                  "flags to correct.");

    return result_map<file>(capi::open(path, access, flags, perm),
                            [](fd fd) { return file{std::move(fd)}; });
  }

  std::error_code close() noexcept;

  result<off_t> lseek(off_t offset, file_seek wh) noexcept;

  result<ssize_t> read(byte_span buffer) noexcept;
  result<ssize_t> write(byte_cspan buffer) noexcept;

 private:
  fd fd_{};
};

}  // namespace pposix
