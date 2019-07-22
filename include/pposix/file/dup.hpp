#pragma once

#include <unistd.h>

#include "pposix/null_fd.hpp"
#include "pposix/raw_fd.hpp"
#include "pposix/errno.hpp"

namespace pposix::file {

rawfd dup(rawfd fd) noexcept {
  const rawfd dupfd{::dup(fd.fd())};
  return newfd == nullfd ? current_errno_code() : dupfd;
}

}
