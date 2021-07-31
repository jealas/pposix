#include "pposix/file.hpp"

#include "pposix/fcntl.hpp"
#include "pposix/util.hpp"

namespace pposix {

file::file(file_descriptor fd) noexcept : fd_{std::move(fd)} {}

std::error_code file::close() noexcept {
  return PPOSIX_COMMON_CALL(::close, static_cast<raw_fd_t>(*fd_));
}

result<off_t> file::lseek(const off_t offset, const file_seek wh) noexcept {
  const auto lseek_count{::lseek(static_cast<raw_fd_t>(*fd_), offset, underlying_v(wh))};
  if (lseek_count == off_t{-1}) {
    return current_errno_code();
  } else {
    return lseek_count;
  }
}

result<ssize_t> file::read(byte_span buffer) noexcept {
    PPOSIX_COMMON_RESULT_CALL_IMPL(::read, static_cast<raw_fd_t>(*fd_), buffer.data(),
                                   buffer.length())}

result<ssize_t> file::write(const byte_cspan buffer) noexcept {
  PPOSIX_COMMON_RESULT_CALL_IMPL(::write, static_cast<raw_fd_t>(*fd_), buffer.data(),
                                 buffer.length())
}
}  // namespace pposix