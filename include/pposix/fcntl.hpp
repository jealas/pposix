#ifndef PPOSIX_FCNTL_HPP
#define PPOSIX_FCNTL_HPP

#include <fcntl.h>

#include "platform.hpp"
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

enum class open_flag : unsigned {
  cloexec = O_CLOEXEC,
  creat = O_CREAT,
  directory = O_DIRECTORY,
  excl = O_EXCL,
  noctty = O_NOCTTY,
  nofollow = O_NOFOLLOW,
  truncate = O_TRUNC,

#if !PPOSIX_PLATFORM_LINUX && !PPOSIX_PLATFORM_OPENBSD
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
  exec = O_PATH,
#else
  exec = O_EXEC,
#endif

  read = O_RDONLY,
  read_write = O_RDWR,

#if !PPOSIX_PLATFORM_LINUX && !PPOSIX_PLATFORM_FREEBSD && !PPOSIX_PLATFORM_OPENBSD
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

}  // namespace capi

template <capi::fcntl_cmd Command>
using fcntl_cmd = enum_flag<capi::fcntl_cmd, Command>;

// fcntl ccommands
inline constexpr fcntl_cmd<capi::fcntl_cmd::dupfd> dupfd{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::dupfd_cloexec> dupfd_cloexec{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::getfd> getfd{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::setfd> setfd{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::getfl> getfl{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::setfl> setfl{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::getlk> getlk{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::setlk> setlk{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::setlkw> setlkw{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::getown> getown{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::setown> setown{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::rdlck> rdlck{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::unlck> unlck{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::wrlck> wrlck{};

#if PPOSIX_PLATFORM_LINUX
inline constexpr fcntl_cmd<capi::fcntl_cmd::ofd_setlk> ofd_setlk{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::ofd_setlkw> ofd_setlkw{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::ofd_getlk> ofd_getlk{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::getown_ex> getown_ex{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::setown_ex> setown_ex{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::getsig> getsig{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::setsig> setsig{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::setlease> setlease{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::getlease> getlease{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::notify> notify{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::setpipe_sz> setpipe_sz{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::getpipe_sz> getpipe_sz{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::add_seals> add_seals{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::get_seals> get_seals{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::seal_seal> seal_seal{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::seal_shrink> seal_shrink{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::seal_grow> seal_grow{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::seal_write> seal_write{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::get_rw_hint> get_rw_hint{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::set_rw_hint> set_rw_hint{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::get_file_rw_hint> get_file_rw_hint{};
inline constexpr fcntl_cmd<capi::fcntl_cmd::set_file_rw_hint> set_file_rw_hint{};
#endif

// Open flags
template <capi::open_flag OpenFlag>
using open_flag = enum_flag<capi::open_flag, OpenFlag>;

inline constexpr open_flag<capi::open_flag::cloexec> cloexec{};
inline constexpr open_flag<capi::open_flag::creat> creat{};
inline constexpr open_flag<capi::open_flag::directory> directory{};
inline constexpr open_flag<capi::open_flag::excl> excl{};
inline constexpr open_flag<capi::open_flag::noctty> noctty{};
inline constexpr open_flag<capi::open_flag::nofollow> nofollow{};
inline constexpr open_flag<capi::open_flag::truncate> truncate{};

#if !PPOSIX_PLATFORM_LINUX && !PPOSIX_PLATFORM_OPENBSD
inline constexpr open_flag<capi::open_flag::tty_init> tty_init{};
#endif

inline constexpr open_flag<capi::open_flag::append> append{};

#if !PPOSIX_PLATFORM_FREEBSD
inline constexpr open_flag<capi::open_flag::dsync> dsync{};
#endif

inline constexpr open_flag<capi::open_flag::nonblock> nonblock{};

#if !PPOSIX_PLATFORM_MACOS && !PPOSIX_PLATFORM_FREEBSD
inline constexpr open_flag<capi::open_flag::rsync> rsync{};
#endif

inline constexpr open_flag<capi::open_flag::sync> sync{};

inline constexpr open_flag<capi::open_flag::rdonly> rdonly{};

// Access mode
template <capi::access_mode AccessMode>
using access_mode = enum_flag<capi::access_mode, AccessMode>;

inline constexpr access_mode<capi::access_mode::exec> exec{};

inline constexpr access_mode<capi::access_mode::read> read{};
inline constexpr access_mode<capi::access_mode::read_write> read_write{};

#if !PPOSIX_PLATFORM_LINUX && !PPOSIX_PLATFORM_FREEBSD && !PPOSIX_PLATFORM_OPENBSD
inline constexpr access_mode<capi::access_mode::search> search{};
#endif

inline constexpr access_mode<capi::access_mode::write> write{};

}  // namespace pposix

#endif  // PPOSIX_FCNTL_HPP
