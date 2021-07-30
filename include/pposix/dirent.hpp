#pragma once

#include <dirent.h>

#include <string_view>
#include <system_error>

#include "pposix/result.hpp"
#include "pposix/unique_d.hpp"
#include "pposix/unique_fd.hpp"

namespace pposix {

std::error_code close_dir(DIR *dir) noexcept;

using dir_fd_t = int;

enum class dir_fd : dir_fd_t {};

struct unique_dir_fd : unique_fd {
  using unique_fd::unique_fd;
};

using unique_dirent = unique_d<DIR *, std::integral_constant<DIR *, nullptr>, close_dir>;

result<unique_dirent> opendir(dir_fd fd) noexcept;

}  // namespace pposix
