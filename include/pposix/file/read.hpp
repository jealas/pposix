#pragma once

#include <cstddef>

#include <unistd.h>

#include "pposix/byte_span.hpp"
#include "pposix/fd.hpp"

namespace pposix::file {

ssize_t read(raw_fd fd, buffer_cspan buffer) noexcept {
  return ::read(fd.fd(), buffer.data(), buffer.length());
}

}
