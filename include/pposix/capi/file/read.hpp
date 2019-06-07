#pragma once

#include <cstddef>

#include <unistd.h>

#include "pposix/capi/rawfd.hpp"

namespace pposix::capi::file {

ssize_t read(rawfd fd, std::byte *data, std::size_t length) noexcept {
  return ::read(fd.fd(), data, length);
}

}  // namespace pposix::capi::file
