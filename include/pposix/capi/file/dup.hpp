#pragma once

#include <unistd.h>

#include "pposix/capi/nullfd.hpp"
#include "pposix/capi/rawfd.hpp"
#include "pposix/errno.hpp"

namespace pposix::capi::file {

rawfd dup(rawfd fd) noexcept {
  const rawfd dupfd{::dup(fd.fd())};
  return newfd == nullfd ? current_errno_code() : dupfd;
}

}  // namespace pposix::capi::file
