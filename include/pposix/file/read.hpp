#ifndef PPOSIX_FILE_READ_HPP
#define PPOSIX_FILE_READ_HPP

#include <cstddef>

#include <unistd.h>

#include "pposix/buffer_span.hpp"
#include "pposix/errno_code.hpp"
#include "pposix/file/fd.hpp"
#include "pposix/result.hpp"
#include "pposix/util/underlying_value.hpp"

namespace pposix::file {

template <class ClosePolicy>
result<buffer_span> read(const file::fd<ClosePolicy> &fd, buffer_span buffer) noexcept {
  const auto bytes_read = ::read(util::underlying_value(fd.raw()),
                                 static_cast<void *>(buffer.data()), buffer.length());
  if (bytes_read < 0) {
    return errno_code();
  } else {
    return buffer.subspan(0u, static_cast<std::size_t>(bytes_read));
  }
}

}  // namespace pposix::file

#endif  // PPOSIX_FILE_READ_HPP
