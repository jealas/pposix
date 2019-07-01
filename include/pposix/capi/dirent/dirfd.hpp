#pragma once

#include <dirent.h>

#include "pposix/capi/nullfd.hpp"
#include "pposix/errno.hpp"
#include "pposix/result.hpp"

namespace pposix::capi::dirent {

result<dir_fd> dirfd(DIR *dir) noexcept {
  const dir_fd fd{::dirfd(dir)};

  if (fd == nullfd) {
    return current_errno_code();
  } else {
    return fd;
  }
};

}