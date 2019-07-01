#pragma once

#include <dirent.h>

#include "pposix/capi/dirent/close_policy.hpp"
#include "pposix/errno.hpp"
#include "pposix/resource.hpp"
#include "pposix/result.hpp"

namespace pposix::capi::dirent {

result<resource<DIR, close_policy>> opendir(char const *dirname) noexcept {
  if (DIR *dir = ::opendir(dirname); dir == nullptr) {
    return current_errno_code();
  } else {
    return resource<DIR, close_policy>{dir};
  }
}

}