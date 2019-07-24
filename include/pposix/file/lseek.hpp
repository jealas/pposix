#pragma once

#include <unistd.h>

#include "pposix/file/offset.hpp"
#include "pposix/file/whence.hpp"
#include "pposix/fd.hpp"
#include "pposix/errno.hpp"
#include "pposix/util/underlying_value.hpp"

namespace pposix::file {

offset_t lseek(raw_fd fd, offset_t offset, whence wh) noexcept {
  return ::lseek(fd.fd(), offset, util::underlying_value(wh));
}

}
