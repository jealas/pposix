#include "pposix/dirent.hpp"

#include "pposix/errno.hpp"
#include "pposix/util.hpp"

namespace pposix {

std::error_code close_dir(DIR *dir) noexcept { return PPOSIX_COMMON_CALL(::closedir, dir); }

result<unique_dirent> opendir(const dir_fd fd) noexcept {
  const auto dirfd = dir_fd_t(fd);

  if (DIR *dir = ::fdopendir(dirfd); dir == nullptr) {
    return current_errno_code();
  } else {
    return unique_dirent{dir};
  }
}

}  // namespace pposix
