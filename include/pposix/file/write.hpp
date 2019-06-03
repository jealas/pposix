#ifndef PPOSIX_FILE_WRITE_HPP
#define PPOSIX_FILE_WRITE_HPP

#include <cstddef>
#include <cstring>
#include <stdexcept>

#include <unistd.h>

#include "pposix/buffer_span.hpp"
#include "pposix/errno_code.hpp"
#include "pposix/file/fd.hpp"
#include "pposix/result.hpp"
#include "pposix/util/underlying_value.hpp"

namespace pposix::file {

template <class ClosePolicy>
result<buffer_span> write(const fd<ClosePolicy> &fd, const buffer_span buffer) {
  const auto bytes_written =
      ::write(util::underlying_value(fd.raw()), buffer.data(), buffer.length());

  if (bytes_written < 0) {
    return errno_code();
  } else if (bytes_written > buffer.length()) {
    throw std::logic_error{"Wrote more bytes than available in the buffer for fd: " +
                           std::to_string(util::underlying_value(fd.raw()))};
  } else {
    return buffer.subspan(0u, static_cast<std::size_t>(bytes_written));
  }
}

}  // namespace pposix::file

#endif  // PPOSIX_FILE_WRITE_HPP
