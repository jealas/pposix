#include "pposix/dirent.hpp"

#include "pposix/errno.hpp"
#include "pposix/util.hpp"

namespace pposix::capi {

std::error_code close_dir(DIR *dir) noexcept { return PPOSIX_COMMON_CALL(::closedir, dir); }

result<dir_descriptor> opendir(dir_fd fd) noexcept {
  if (DIR *dir = ::fdopendir(static_cast<raw_dir_fd_t>(fd.release())); dir == nullptr) {
    return current_errno_code();
  } else {
    return dir_descriptor{dir};
  }
}

result<dir_descriptor> opendir(const char *path) noexcept {
  if (DIR *dir = ::opendir(path); dir == nullptr) {
    return current_errno_code();
  } else {
    return dir_descriptor{dir};
  }
}

}  // namespace pposix::capi
