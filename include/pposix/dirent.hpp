#pragma once

#include <dirent.h>

#include <string_view>
#include <system_error>

#include "pposix/descriptor.hpp"
#include "pposix/file_descriptor.hpp"
#include "pposix/result.hpp"

namespace pposix {

std::error_code close_dir(DIR *dir) noexcept;

using dir_fd_t = int;

enum class dir_fd : dir_fd_t {};

struct unique_dir_fd : file_descriptor {
  using file_descriptor::file_descriptor;
};

using unique_dirent = descriptor<DIR *, std::integral_constant<DIR *, nullptr>, close_dir>;

result<unique_dirent> opendir(dir_fd fd) noexcept;
result<unique_dirent> opendir(const char *dir) noexcept;

}  // namespace pposix
