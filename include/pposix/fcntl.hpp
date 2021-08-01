#ifndef PPOSIX_FCNTL_HPP
#define PPOSIX_FCNTL_HPP

#include <fcntl.h>

#include "fd.hpp"
#include "platform.hpp"
#include "result.hpp"
#include "stat.hpp"
#include "util.hpp"

namespace pposix::capi {
enum class fcntl_cmd : int {
  dupfd = F_DUPFD,
  dupfd_cloexec = F_DUPFD_CLOEXEC,
  getfd = F_GETFD,
  setfd = F_SETFD,
  getfl = F_GETFL,
  setfl = F_SETFL,
  getlk = F_GETLK,
  setlk = F_SETLK,
  setlkw = F_SETLKW,
  getown = F_GETOWN,
  setown = F_SETOWN,
  rdlck = F_RDLCK,
  unlck = F_UNLCK,
  wrlck = F_WRLCK,

#if PPOSIX_PLATFORM_LINUX
  ofd_setlk = F_OFD_SETLK,
  ofd_setlkw = F_OFD_SETLKW,
  ofd_getlk = F_OFD_GETLK,
  getown_ex = F_GETOWN_EX,
  setown_ex = F_SETOWN_EX,
  getsig = F_GETSIG,
  setsig = F_SETSIG,
  setlease = F_SETLEASE,
  getlease = F_GETLEASE,
  notify = F_NOTIFY,
  setpipe_sz = F_SETPIPE_SZ,
  getpipe_sz = F_GETPIPE_SZ,
  add_seals = F_ADD_SEALS,
  get_seals = F_GET_SEALS,
  seal_seal = F_SEAL_SEAL,
  seal_shrink = F_SEAL_SHRINK,
  seal_grow = F_SEAL_GROW,
  seal_write = F_SEAL_WRITE,
  get_rw_hint = F_GET_RW_HINT,
  set_rw_hint = F_SET_RW_HINT,
  get_file_rw_hint = F_GET_FILE_RW_HINT,
  set_file_rw_hint = F_GET_FILE_RW_HINT,
#endif
};

constexpr fcntl_cmd operator|(fcntl_cmd lhs, fcntl_cmd rhs) noexcept {
  return fcntl_cmd{underlying_v(lhs) | underlying_v(rhs)};
}

constexpr fcntl_cmd &operator|=(fcntl_cmd &lhs, fcntl_cmd rhs) noexcept {
  lhs = lhs | rhs;
  return lhs;
}

std::error_code fcntl(capi::fcntl_cmd cmd) noexcept;
std::error_code fcntl(capi::fcntl_cmd cmd, int arg) noexcept;
std::error_code fcntl(capi::fcntl_cmd cmd, void *arg) noexcept;

enum class open_flag : unsigned {
  cloexec = O_CLOEXEC,
  creat = O_CREAT,
  directory = O_DIRECTORY,
  excl = O_EXCL,
  noctty = O_NOCTTY,
  nofollow = O_NOFOLLOW,
  truncate = O_TRUNC,

#if !PPOSIX_PLATFORM_LINUX && !PPOSIX_PLATFORM_OPENBSD && !PPOSIX_PLATFORM_MACOS
  tty_init = O_TTY_INIT
#endif

      append = O_APPEND,

#if !PPOSIX_PLATFORM_FREEBSD
  dsync = O_DSYNC,
#endif

  nonblock = O_NONBLOCK,

#if !PPOSIX_PLATFORM_MACOS && !PPOSIX_PLATFORM_FREEBSD
  rsync = O_RSYNC,
#endif

  sync = O_SYNC,

  rdonly = O_RDONLY,

};

constexpr open_flag operator|(open_flag lhs, open_flag rhs) noexcept {
  return open_flag{underlying_v(lhs) | underlying_v(rhs)};
}

constexpr open_flag &operator|=(open_flag &lhs, open_flag rhs) noexcept {
  lhs = lhs | rhs;
  return lhs;
}

enum class access_mode : unsigned {
#if PPOSIX_PLATFORM_LINUX
  // O_PATH is equivalent to O_EXEC on Linux
  exec = O_PATH,
#elif !PPOSIX_PLATFORM_MACOS && !PPOSIX_PLATFORM_OPENBSD
  exec = O_EXEC,
#endif

  read = O_RDONLY,
  read_write = O_RDWR,

#if !PPOSIX_PLATFORM_LINUX && !PPOSIX_PLATFORM_FREEBSD && !PPOSIX_PLATFORM_OPENBSD && \
    !PPOSIX_PLATFORM_MACOS
  search = O_SEARCH,
#endif

  write = O_WRONLY,

  accmode = O_ACCMODE
};

constexpr access_mode operator|(access_mode lhs, access_mode rhs) noexcept {
  return access_mode{underlying_v(lhs) | underlying_v(rhs)};
}

constexpr access_mode &operator|=(access_mode &lhs, access_mode rhs) noexcept {
  lhs = lhs | rhs;
  return lhs;
}

result<fd> open(const char *, capi::access_mode, capi::open_flag) noexcept;
result<fd> open(const char *, capi::access_mode, capi::open_flag, capi::permission) noexcept;

}  // namespace pposix::capi

#endif  // PPOSIX_FCNTL_HPP
