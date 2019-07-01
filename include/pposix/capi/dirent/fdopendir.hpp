#pragma once

#include <dirent.h>

#include "pposix/capi/dirent/close_policy.hpp"
#include "pposix/capi/dirent/dir_fd.hpp"
#include "pposix/errno.hpp"
#include "pposix/resource.hpp"
#include "pposix/result.hpp"
#include "pposix/unique_fd.hpp"

namespace pposix::capi::dirent {

template <class ClosePolicy>
result<resource<DIR, close_policy>> fdopendir(unique_fd<dir_fd, ClosePolicy> fd) noexcept {
  const auto fd = fd.release();

  if (DIR *dir = ::fdopendir(fd.fd()); dir == nullptr) {
    return current_errno_code();
  } else {
    return resource<DIR, close_policy>{dir};
  }
}

}