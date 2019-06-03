#ifndef PPOSIX_FILE_LSEEK_HPP
#define PPOSIX_FILE_LSEEK_HPP

#include <unistd.h>

#include "pposix/errno_code.hpp"
#include "pposix/file/fd.hpp"
#include "pposix/file/offset.hpp"
#include "pposix/file/seek_whence.hpp"
#include "pposix/result.hpp"
#include "pposix/util/underlying_value.hpp"

namespace pposix::file {

template <class ClosePolicy>
result<offset_t> lseek(const fd<ClosePolicy> &fd, offset_t offset, seek_whence whence) {
  const auto offset =
      ::lseek(util::underlying_value(fd.raw()), offset, util::underlying_value(whence));

  if (offset == static_cast<offset_t>(-1)) {
    return errno_code();
  } else {
    return offset;
  }
}

}  // namespace pposix::file

#endif  // PPOSIX_FILE_LSEEK_HPP
