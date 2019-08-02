#include "pposix/dirent.hpp"

#include "pposix/errno.hpp"

namespace pposix {

// Dirent close dir
std::error_code closedir(DIR *dir) noexcept {
  return ::closedir(dir) == -1 ? current_errno_code() : std::error_code{};
}

// Dirent close policy
std::error_code dirent_close_policy::operator()(DIR *dir) const noexcept {
  return pposix::closedir(dir);
}

// Dirent open directory
result<resource<DIR, dirent_close_policy>> opendir(char const *dirname) noexcept {
  if (DIR *dir = ::opendir(dirname); dir == nullptr) {
    return current_errno_code();
  } else {
    return resource<DIR, dirent_close_policy>{dir};
  }
}

result<resource<DIR, dirent_close_policy>> opendir(const std::string &dirname) noexcept {
  return pposix::opendir(dirname.c_str());
}

// Dirent get directory file descriptor
result<unique_fd<dir_fd>> dirfd(DIR *dir) noexcept {
  dir_fd fd{::dirfd(dir)};
  if (fd == nullfd) {
    return current_errno_code();
  } else {
    return unique_fd<dir_fd>{std::move(fd)};
  }
}

}  // namespace pposix
