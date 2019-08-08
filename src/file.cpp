#include "pposix/file.hpp"

namespace pposix {

// File open: file
result<unique_fd<raw_fd>> open(const char *path, file_mode mode, file_flags flags) noexcept {
  const raw_fd new_fd{::open(path, underlying_value(mode) | underlying_value(flags))};
  if (new_fd == nullfd) {
    return current_errno_code();
  } else {
    return unique_fd<raw_fd>{new_fd};
  }
}

result<unique_fd<raw_fd>> open(const std::filesystem::path &path, file_mode mode,
                               file_flags flags) noexcept {
  return pposix::open(path.c_str(), mode, flags);
}

// File close
std::error_code close(raw_fd fd) noexcept {
  return ::close(fd.raw()) == -1 ? current_errno_code() : std::error_code{};
}

// File dup
result<unique_fd<raw_fd>> dup(raw_fd fd) noexcept {
  const raw_fd newfd{::dup(fd.raw())};
  if (newfd == nullfd) {
    return current_errno_code();
  } else {
    return unique_fd<raw_fd>{newfd};
  }
}

// File lseek
result<off_t> lseek(raw_fd fd, off_t offset, file_seek wh) noexcept {
  const auto lseek_count{::lseek(fd.raw(), offset, underlying_value(wh))};
  if (lseek_count == off_t{-1}) {
    return current_errno_code();
  } else {
    return lseek_count;
  }
}

// File read
result<ssize_t> read(raw_fd fd, byte_span buffer) noexcept {
  const auto bytes_read{::read(fd.raw(), buffer.data(), buffer.length())};
  if (bytes_read == -1) {
    return current_errno_code();
  } else {
    return bytes_read;
  }
}

// File write
result<ssize_t> write(raw_fd fd, byte_cspan buffer) noexcept {
  const auto bytes_written{::write(fd.raw(), buffer.data(), buffer.length())};
  if (bytes_written == -1) {
    return current_errno_code();
  } else {
    return bytes_written;
  }
}

}