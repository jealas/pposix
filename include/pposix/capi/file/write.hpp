#pragma once

#include <cstddef>

#include <unistd.h>

#include "pposix/capi/rawfd.hpp"
#include "pposix/buffer_span.hpp"

namespace pposix::capi::file {

ssize_t write(rawfd fd, ) noexcept {
  return ::write(fd.fd(), data, length);
}

}  // namespace pposix::capi::file
