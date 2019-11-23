#ifndef PPOSIX_FCNTL_HPP
#define PPOSIX_FCNTL_HPP

#include <fcntl.h>

#include "platform.hpp"

namespace pposix {

namespace capi {

enum class fcntl_cmd : int {
  dupfd = F_DUPFD,
  getfd = F_GETFD,
  setfd = F_SETFD,
  getfl = F_GETFL,
  setfl = F_SETFL,
  getown = F_GETOWN,
  setown = F_SETOWN,
  getlk = F_GETLK,
  setlk = F_SETLK,
  setlkw = F_SETLKW,

#if PPOSIX_PLATFORM_LINUX
  dupfd_cloexec = F_DUPFD_CLOEXEC,
  ofd_setlk = F_OFD_SETLK,
  ofd_setlkw = F_OFD_SETLKW,
  ofd_getlk = F_OFD_GETLK,
  getown_ex = F_GETOWN_EX,
  setown_ex = F_SETOWN_EX,
  getsig = F_GETSIG,
  setsig = F_SETSIG,
  setlease = F_SETLEASE,
  getlease = F_GETLEASE,
  rdlck = F_RDLCK,
  wrlck = F_WRLCK,
  unlck = F_UNLCK,
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

}

}  // namespace pposix

#endif  // PPOSIX_FCNTL_HPP
