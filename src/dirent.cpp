#include "pposix/dirent.hpp"

#include "pposix/errno.hpp"

namespace pposix {

dir_fd::dir_fd(int fd) noexcept : dir_fd_{fd} {}

raw_fd dir_fd::get() const noexcept { return *dir_fd_; }
raw_fd dir_fd::release() noexcept { return dir_fd_.release(); }

dirent::dirent(DIR *dir) noexcept : dirent_d_{dir} {}

DIR *dirent::get() noexcept { return *dirent_d_; }

DIR *dirent::release() noexcept { return dirent_d_.release(); }

result<dirent> dirent::opendir(char const *dirname) noexcept {
  if (DIR *dir = ::opendir(dirname); dir == nullptr) {
    return current_errno_code();
  } else {
    return dirent{dir};
  }
}

std::error_code dirent_close_policy::operator()(DIR *dir) const noexcept {
  return ::closedir(dir) == -1 ? current_errno_code() : std::error_code{};
}

}  // namespace pposix
