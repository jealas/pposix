#pragma once

#include <cstddef>

#include <unistd.h>

#include "pposix/byte_span.hpp"
#include "pposix/errno.hpp"
#include "pposix/fd.hpp"
#include "pposix/result.hpp"

namespace pposix::file {

result<ssize_t> read(raw_fd fd, buffer_cspan buffer) noexcept {
  const auto bytes_read{::read(fd.raw(), buffer.data(), buffer.length())};
  if (bytes_read == -1) {
    return current_errno_code();
  } else {
    return bytes_read;
  }
}

}  // namespace pposix::file
