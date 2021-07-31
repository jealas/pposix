#include "pposix/dirent.hpp"

#include "pposix/errno.hpp"
#include "pposix/util.hpp"

namespace pposix {

std::error_code close_dir(DIR *dir) noexcept { return PPOSIX_COMMON_CALL(::closedir, dir); }

result<unique_dirent> opendir(const dir_fd fd) noexcept {
  if (DIR *dir = ::fdopendir(static_cast<dir_fd_t>(fd)); dir == nullptr) {
    return current_errno_code();
  } else {
    return unique_dirent{dir};
  }
}

result<unique_dirent> opendir(const char *path) noexcept
{
  if (DIR *dir = ::opendir(path); dir == nullptr) {
    return current_errno_code();
  } else {
    return unique_dirent{dir};
  }
}

}  // namespace pposix
