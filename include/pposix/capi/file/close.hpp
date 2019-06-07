#pragma once

#include <system_error>

#include <unistd.h>

#include "pposix/capi/rawfd.hpp"
#include "pposix/errno_code.hpp"

namespace pposix::capi::file {

std::error_code close(rawfd fd) noexcept {
  const auto error = ::close(fd.fd());
  return error == -1 ? errno_code() : {};
}

}  // namespace pposix::capi::file
