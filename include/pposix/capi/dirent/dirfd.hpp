#pragma once

#include <dirent.h>

#include "pposix/capi/nullfd.hpp"
#include "pposix/errno_code.hpp"
#include "pposix/result.hpp"

namespace pposix::capi::dirent {

result<dir_fd> dirfd(DIR *dir) noexcept {
  const dir_fd fd{::dirfd(dir)};

  if (fd == nullfd) {
    return errno_code();
  } else {
    return fd;
  }
};

}