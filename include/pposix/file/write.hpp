#pragma once

#include <cstddef>

#include <unistd.h>

#include "pposix/byte_span.hpp"
#include "pposix/errno.hpp"
#include "pposix/fd.hpp"
#include "pposix/result.hpp"

namespace pposix::file {

result<ssize_t> write(raw_fd fd, buffer_cspan buffer) noexcept {
  const auto bytes_written{::write(fd.raw(), buffer.data(), buffer.length())};
  if (bytes_written == -1) {
    return current_errno_code();
  } else {
    return bytes_written;
  }
}

}  // namespace pposix::file
