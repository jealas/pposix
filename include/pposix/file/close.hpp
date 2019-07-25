#pragma once

#include <system_error>

#include <unistd.h>

#include "pposix/errno.hpp"
#include "pposix/fd.hpp"

namespace pposix::file {

std::error_code close(raw_fd fd) noexcept {
  return ::close(fd.raw()) == -1 ? current_errno_code() : std::error_code{};
}

}  // namespace pposix::file
