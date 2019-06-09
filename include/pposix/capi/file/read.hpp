#pragma once

#include <cstddef>

#include <unistd.h>

#include "pposix/byte_span.hpp"
#include "pposix/capi/rawfd.hpp"

namespace pposix::capi::file {

ssize_t read(rawfd fd, buffer_cspan buffer) noexcept {
  return ::read(fd.fd(), buffer.data(), buffer.length());
}

}  // namespace pposix::capi::file
