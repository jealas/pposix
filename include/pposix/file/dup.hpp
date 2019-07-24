#pragma once

#include <unistd.h>

#include "pposix/errno.hpp"
#include "pposix/fd.hpp"
#include "pposix/null_fd.hpp"
#include "pposix/unique_fd.hpp"

namespace pposix::file {

unique_fd<raw_fd> dup(raw_fd fd) noexcept {
  const raw_fd dupfd{::dup(fd.fd())};
  return newfd == nullfd ? current_errno_code() : dupfd;
}

}  // namespace pposix::file
