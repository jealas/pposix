#pragma once

#include <system_error>

#include <dirent.h>

#include "pposix/errno.hpp"

namespace pposix::capi::dirent {

std::error_code closedir(DIR *dir) noexcept {
  if (const auto error = ::closedir(dir); error == -1) {
    return current_errno_code();
  } else {
    return {};
  }
}

}