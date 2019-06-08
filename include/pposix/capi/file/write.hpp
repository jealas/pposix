#pragma once

#include <cstddef>

#include <unistd.h>

#include "pposix/buffer_span.hpp"
#include "pposix/capi/rawfd.hpp"

namespace pposix::capi::file {

ssize_t write(rawfd fd, buffer_cspan buffer) noexcept {
  return ::write(fd.fd(), buffer.data(), buffer.length());
}

}  // namespace pposix::capi::file
