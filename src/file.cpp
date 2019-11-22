#include "pposix/file.hpp"

namespace pposix {

file::file(raw_fd fd) noexcept : fd_{fd} {}

result<unique_fd> file::open(const char *path, file_mode mode, file_flags flags) noexcept {
  const raw_fd new_fd{::open(path, underlying_value(mode) | underlying_value(flags))};
  if (new_fd == -1) {
    return current_errno_code();
  } else {
    return unique_fd{new_fd};
  }
}

std::error_code file::close() noexcept {
  return ::close(*fd_) == -1 ? current_errno_code() : std::error_code{};
}

result<off_t> file::lseek(off_t offset, file_seek wh) noexcept {
  const auto lseek_count{::lseek(*fd_, offset, underlying_value(wh))};
  if (lseek_count == off_t{-1}) {
    return current_errno_code();
  } else {
    return lseek_count;
  }
}

result<ssize_t> file::read(byte_span buffer) noexcept {
  const auto bytes_read{::read(*fd_, buffer.data(), buffer.length())};
  if (bytes_read == -1) {
    return current_errno_code();
  } else {
    return bytes_read;
  }
}

result<ssize_t> file::write(byte_cspan buffer) noexcept {
  const auto bytes_written{::write(*fd_, buffer.data(), buffer.length())};
  if (bytes_written == -1) {
    return current_errno_code();
  } else {
    return bytes_written;
  }
}

}