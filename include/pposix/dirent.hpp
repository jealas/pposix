#pragma once

#include <string_view>
#include <system_error>

#include <dirent.h>

#include "pposix/fd.hpp"
#include "pposix/resource.hpp"
#include "pposix/result.hpp"
#include "pposix/unique_fd.hpp"

namespace pposix {

// Direct file descriptor
struct dir_fd_tag {};
using dir_fd = fd<dir_fd_tag>;

// Dirent close directory
std::error_code closedir(DIR *dir) noexcept;

// Dirent close policy
struct dirent_close_policy {
  std::error_code operator()(DIR *dir) const noexcept;
};

// Dirent open directory
result<resource<DIR, dirent_close_policy>> opendir(char const *dirname) noexcept;
result<resource<DIR, dirent_close_policy>> opendir(const std::string &dirname) noexcept;

// Dirent open directory from file descriptor
template <class ClosePolicy>
result<resource<DIR, dirent_close_policy>> fdopendir(unique_fd<dir_fd, ClosePolicy> fd) noexcept {
  const auto dirfd = fd.release();

  if (DIR *dir = ::fdopendir(dirfd.raw()); dir == nullptr) {
    return current_errno_code();
  } else {
    return resource<DIR, dirent_close_policy>{dir};
  }
}

// Dirent get directory file descriptor
result<unique_fd<dir_fd>> dirfd(DIR *dir) noexcept;

}  // namespace pposix
