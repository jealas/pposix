#pragma once

#include <system_error>

#include <unistd.h>

#include "pposix/raw_fd.hpp"
#include "pposix/errno.hpp"

namespace pposix::file {

std::error_code close(rawfd fd) noexcept {
  const auto error = ::close(fd.fd());
  return error == -1 ? current_errno_code() : {};
}

}
