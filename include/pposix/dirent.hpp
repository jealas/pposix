#pragma once

#include <dirent.h>

#include <string_view>
#include <system_error>

#include "pposix/descriptor.hpp"
#include "pposix/fd.hpp"
#include "pposix/result.hpp"

namespace pposix {

std::error_code close_dir(DIR *dir) noexcept;

using dir_descriptor = descriptor<DIR *, std::integral_constant<DIR *, nullptr>, close_dir>;

result<dir_descriptor> opendir(raw_dir_fd fd) noexcept;
result<dir_descriptor> opendir(const char *dir) noexcept;

}  // namespace pposix
