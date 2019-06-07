#pragma once

#include <cstddef>

#include <unistd.h>

#include "pposix/capi/rawfd.hpp"

namespace pposix::capi::file {

ssize_t write(rawfd fd, std::byte const *data, std::size_t length) noexcept {
  return ::write(fd.fd(), data, length);
}

}  // namespace pposix::capi::file
