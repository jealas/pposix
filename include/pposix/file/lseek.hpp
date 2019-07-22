#pragma once

#include <unistd.h>

#include "pposix/capi/file/offset.hpp"
#include "pposix/capi/file/whence.hpp"
#include "pposix/capi/raw_fd.hpp"
#include "pposix/errno.hpp"
#include "pposix/util/underlying_value.hpp"

namespace pposix::capi::file {

offset_t lseek(rawfd fd, offset_t offset, whence wh) noexcept {
  return ::lseek(fd.fd(), offset, util::underlying_value(wh));
}

}  // namespace pposix::capi::file
