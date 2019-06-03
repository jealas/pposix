#ifndef PPOSIX_FILE_READ_HPP
#define PPOSIX_FILE_READ_HPP

#include <cstddef>
#include <stdexcept>
#include <string>

#include <unistd.h>

#include "pposix/buffer_span.hpp"
#include "pposix/errno_code.hpp"
#include "pposix/file/fd.hpp"
#include "pposix/result.hpp"
#include "pposix/util/underlying_value.hpp"

namespace pposix::file {

template <class ClosePolicy>
result<buffer_span> read(const fd<ClosePolicy> &fd, buffer_span buffer) {
  const auto bytes_read = ::read(util::underlying_value(fd.raw()),
                                 static_cast<void *>(buffer.data()), buffer.length());
  if (bytes_read == -1) {
    return errno_code();
  } else if (bytes_read < 0) {
    throw std::logic_error{"Read returned non-error negative bytes: " +
                           std::to_string(bytes_read)};
  } else if (bytes_read > buffer.length()) {
    throw std::logic_error{"Read more bytes than available in the buffer"};
  } else {
    return buffer.subspan(0u, static_cast<std::size_t>(bytes_read));
  }
}

}  // namespace pposix::file

#endif  // PPOSIX_FILE_READ_HPP
