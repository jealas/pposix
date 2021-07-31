#pragma once

#include <dirent.h>

#include <string_view>
#include <system_error>

#include "pposix/file_descriptor.hpp"
#include "pposix/result.hpp"
#include "pposix/unique_descriptor.hpp"

namespace pposix {

std::error_code close_dir(DIR *dir) noexcept;

using dir_fd_t = int;

enum class dir_fd : dir_fd_t {};

struct unique_dir_fd : unique_fd {
  using unique_fd::unique_fd;
};

using unique_dirent = unique_descriptor<DIR *, std::integral_constant<DIR *, nullptr>, close_dir>;

result<unique_dirent> opendir(dir_fd fd) noexcept;
result<unique_dirent> opendir(const char *dir) noexcept;

}  // namespace pposix
