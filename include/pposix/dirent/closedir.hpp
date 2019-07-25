#pragma once

#include <system_error>

#include <dirent.h>

#include "pposix/errno.hpp"

namespace pposix::dirent {

std::error_code closedir(DIR *dir) noexcept {
  return ::closedir(dir) == -1 ? current_errno_code() : {};
}

}