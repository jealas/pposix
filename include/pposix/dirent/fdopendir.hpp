#pragma once

#include <dirent.h>

#include "pposix/dirent/close_policy.hpp"
#include "pposix/dirent/dir_fd.hpp"
#include "pposix/errno.hpp"
#include "pposix/resource.hpp"
#include "pposix/result.hpp"
#include "pposix/unique_fd.hpp"

namespace pposix::dirent {

template <class ClosePolicy>
result<resource<DIR, close_policy>> fdopendir(unique_fd<dir_fd, ClosePolicy> fd) noexcept {
  const auto fd = fd.release();

  if (DIR *dir = ::fdopendir(fd.raw()); dir == nullptr) {
    return current_errno_code();
  } else {
    return resource<DIR, close_policy>{dir};
  }
}

}