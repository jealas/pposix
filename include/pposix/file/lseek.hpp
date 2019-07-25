#pragma once

#include <unistd.h>

#include "pposix/errno.hpp"
#include "pposix/fd.hpp"
#include "pposix/file/offset.hpp"
#include "pposix/file/whence.hpp"
#include "pposix/util/underlying_value.hpp"

namespace pposix::file {

result<off_t> lseek(raw_fd fd, offset_t offset, whence wh) noexcept {
  const auto lseek_count{::lseek(fd.raw(), offset, util::underlying_value(wh))};
  if (lseek_count == off_t{-1}) {
    return current_errno_code();
  } else {
    return lseek_count;
  }
}

}  // namespace pposix::file
