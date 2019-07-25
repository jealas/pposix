#pragma once

#include <unistd.h>

#include "pposix/errno.hpp"
#include "pposix/fd.hpp"
#include "pposix/null_fd.hpp"
#include "pposix/result.hpp"
#include "pposix/unique_fd.hpp"

namespace pposix::file {

result<unique_fd<raw_fd>> dup(raw_fd fd) noexcept {
  const raw_fd newfd{::dup(fd.raw())};
  if (newfd == nullfd) {
    return current_errno_code();
  } else {
    return unique_fd<raw_fd>{dupfd};
  }
}

}  // namespace pposix::file
