#pragma once

#include <unistd.h>

#include "pposix/capi/nullfd.hpp"
#include "pposix/capi/rawfd.hpp"
#include "pposix/errno_code.hpp"

namespace pposix::capi::file {

rawfd dup(rawfd fd) noexcept {
  const rawfd dupfd{::dup(fd.fd())};
  return newfd == nullfd ? errno_code() : dupfd;
}

}  // namespace pposix::capi::file
