#ifndef PPOSIX_FCNTL_HPP
#define PPOSIX_FCNTL_HPP

#include <fcntl.h>

#include "file_descriptor.hpp"
#include "platform.hpp"
#include "result.hpp"
#include "stat.hpp"
#include "util.hpp"

namespace pposix {

namespace capi {

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
#elif !PPOSIX_PLATFORM_MACOS
  exec = O_EXEC,
#endif

  read = O_RDONLY,
  read_write = O_RDWR,

#if !PPOSIX_PLATFORM_LINUX && !PPOSIX_PLATFORM_FREEBSD && !PPOSIX_PLATFORM_OPENBSD && !PPOSIX_PLATFORM_MACOS
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

result<raw_fd> open(const char *path, capi::access_mode mode, capi::open_flag flags) noexcept;

result<raw_fd> open(const char *path, capi::access_mode mode, capi::open_flag flags,
                    capi::permission permission) noexcept;

}  // namespace capi

template <capi::fcntl_cmd Command>
using fcntl_cmd = enum_flag<capi::fcntl_cmd, Command>;

// fcntl ccommands
constexpr fcntl_cmd<capi::fcntl_cmd::dupfd> dupfd{};
constexpr fcntl_cmd<capi::fcntl_cmd::dupfd_cloexec> dupfd_cloexec{};
constexpr fcntl_cmd<capi::fcntl_cmd::getfd> getfd{};
constexpr fcntl_cmd<capi::fcntl_cmd::setfd> setfd{};
constexpr fcntl_cmd<capi::fcntl_cmd::getfl> getfl{};
constexpr fcntl_cmd<capi::fcntl_cmd::setfl> setfl{};
constexpr fcntl_cmd<capi::fcntl_cmd::getlk> getlk{};
constexpr fcntl_cmd<capi::fcntl_cmd::setlk> setlk{};
constexpr fcntl_cmd<capi::fcntl_cmd::setlkw> setlkw{};
constexpr fcntl_cmd<capi::fcntl_cmd::getown> getown{};
constexpr fcntl_cmd<capi::fcntl_cmd::setown> setown{};
constexpr fcntl_cmd<capi::fcntl_cmd::rdlck> rdlck{};
constexpr fcntl_cmd<capi::fcntl_cmd::unlck> unlck{};
constexpr fcntl_cmd<capi::fcntl_cmd::wrlck> wrlck{};

#if PPOSIX_PLATFORM_LINUX
constexpr fcntl_cmd<capi::fcntl_cmd::ofd_setlk> ofd_setlk{};
constexpr fcntl_cmd<capi::fcntl_cmd::ofd_setlkw> ofd_setlkw{};
constexpr fcntl_cmd<capi::fcntl_cmd::ofd_getlk> ofd_getlk{};
constexpr fcntl_cmd<capi::fcntl_cmd::getown_ex> getown_ex{};
constexpr fcntl_cmd<capi::fcntl_cmd::setown_ex> setown_ex{};
constexpr fcntl_cmd<capi::fcntl_cmd::getsig> getsig{};
constexpr fcntl_cmd<capi::fcntl_cmd::setsig> setsig{};
constexpr fcntl_cmd<capi::fcntl_cmd::setlease> setlease{};
constexpr fcntl_cmd<capi::fcntl_cmd::getlease> getlease{};
constexpr fcntl_cmd<capi::fcntl_cmd::notify> notify{};
constexpr fcntl_cmd<capi::fcntl_cmd::setpipe_sz> setpipe_sz{};
constexpr fcntl_cmd<capi::fcntl_cmd::getpipe_sz> getpipe_sz{};
constexpr fcntl_cmd<capi::fcntl_cmd::add_seals> add_seals{};
constexpr fcntl_cmd<capi::fcntl_cmd::get_seals> get_seals{};
constexpr fcntl_cmd<capi::fcntl_cmd::seal_seal> seal_seal{};
constexpr fcntl_cmd<capi::fcntl_cmd::seal_shrink> seal_shrink{};
constexpr fcntl_cmd<capi::fcntl_cmd::seal_grow> seal_grow{};
constexpr fcntl_cmd<capi::fcntl_cmd::seal_write> seal_write{};
constexpr fcntl_cmd<capi::fcntl_cmd::get_rw_hint> get_rw_hint{};
constexpr fcntl_cmd<capi::fcntl_cmd::set_rw_hint> set_rw_hint{};
constexpr fcntl_cmd<capi::fcntl_cmd::get_file_rw_hint> get_file_rw_hint{};
constexpr fcntl_cmd<capi::fcntl_cmd::set_file_rw_hint> set_file_rw_hint{};
#endif

// Open flags
template <capi::open_flag OpenFlag>
using open_flag = enum_flag<capi::open_flag, OpenFlag>;

constexpr open_flag<capi::open_flag::cloexec> cloexec{};
constexpr open_flag<capi::open_flag::creat> creat{};
constexpr open_flag<capi::open_flag::directory> directory{};
constexpr open_flag<capi::open_flag::excl> excl{};
constexpr open_flag<capi::open_flag::noctty> noctty{};
constexpr open_flag<capi::open_flag::nofollow> nofollow{};
constexpr open_flag<capi::open_flag::truncate> truncate{};

#if !PPOSIX_PLATFORM_LINUX && !PPOSIX_PLATFORM_OPENBSD && !PPOSIX_PLATFORM_MACOS
constexpr open_flag<capi::open_flag::tty_init> tty_init{};
#endif

constexpr open_flag<capi::open_flag::append> append{};

#if !PPOSIX_PLATFORM_FREEBSD
constexpr open_flag<capi::open_flag::dsync> dsync{};
#endif

constexpr open_flag<capi::open_flag::nonblock> nonblock{};

#if !PPOSIX_PLATFORM_MACOS && !PPOSIX_PLATFORM_FREEBSD
constexpr open_flag<capi::open_flag::rsync> rsync{};
#endif

constexpr open_flag<capi::open_flag::sync> sync{};

constexpr open_flag<capi::open_flag::rdonly> rdonly{};

// Access mode
template <capi::access_mode AccessMode>
using access_mode = enum_flag<capi::access_mode, AccessMode>;

#if !PPOSIX_PLATFORM_MACOS
constexpr access_mode<capi::access_mode::exec> exec{};
#endif

constexpr access_mode<capi::access_mode::read> read{};
constexpr access_mode<capi::access_mode::read_write> read_write{};

#if !PPOSIX_PLATFORM_LINUX && !PPOSIX_PLATFORM_FREEBSD && !PPOSIX_PLATFORM_OPENBSD && !PPOSIX_PLATFORM_MACOS
constexpr access_mode<capi::access_mode::search> search{};
#endif

constexpr access_mode<capi::access_mode::write> write{};

}  // namespace pposix

#endif  // PPOSIX_FCNTL_HPP
