#pragma once

#include <dirent.h>

#include "pposix/null_fd.hpp"
#include "pposix/errno.hpp"
#include "pposix/result.hpp"
#include "pposix/dirent/dir_fd.hpp"

namespace pposix::dirent {

result<dir_fd> dirfd(DIR *dir) noexcept {
  const dir_fd fd{::dirfd(dir)};

  if (fd == nullfd) {
    return current_errno_code();
  } else {
    return fd;
  }
};

}