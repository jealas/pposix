#pragma once

#include <dirent.h>

#include "pposix/dirent/dir_fd.hpp"
#include "pposix/errno.hpp"
#include "pposix/null_fd.hpp"
#include "pposix/result.hpp"
#include "pposix/unique_fd.hpp"

namespace pposix::dirent {

result<unique_fd<dir_fd>> dirfd(DIR *dir) noexcept {
  const dir_fd fd{::dirfd(dir)};
  return fd == nullfd ? current_errno_code() : unique_fd<dir_fd>{fd};
};

}