#include "pposix/file.hpp"

#include "pposix/util.hpp"

namespace pposix {

file::file(raw_fd fd) noexcept : fd_{fd} {}

result<raw_fd> file::unsafe_open(const char *path, file_mode mode, file_flags flags) noexcept {
  return PPOSIX_COMMON_CALL(::open, path, underlying_v(mode) | underlying_v(flags));
}

result<file> file::open(const char *path, file_mode mode, file_flags flags) noexcept {
  return result_map<file>(unsafe_open(path, mode, flags),
                          [](const raw_fd &fd) { return file{fd}; });
}

std::error_code file::close() noexcept { return PPOSIX_COMMON_CALL(::close, *fd_); }

result<off_t> file::lseek(off_t offset, file_seek wh) noexcept {
  const auto lseek_count{::lseek(*fd_, offset, underlying_v(wh))};
  if (lseek_count == off_t{-1}) {
    return current_errno_code();
  } else {
    return lseek_count;
  }
}

result<ssize_t> file::read(byte_span buffer) noexcept {
    PPOSIX_COMMON_RESULT_CALL_IMPL(::read, *fd_, buffer.data(), buffer.length())}

result<ssize_t> file::write(byte_cspan buffer) noexcept {
    PPOSIX_COMMON_RESULT_CALL_IMPL(::write, *fd_, buffer.data(), buffer.length())}

std::error_code file::unsafe_fcntl(const capi::fcntl_cmd cmd) const noexcept {
  return PPOSIX_COMMON_CALL(::fcntl, *fd_, underlying_v(cmd));
}

std::error_code file::unsafe_fcntl(const capi::fcntl_cmd cmd, const int arg) const noexcept {
  return PPOSIX_COMMON_CALL(::fcntl, *fd_, underlying_v(cmd), arg);
}

std::error_code file::unsafe_fcntl(const capi::fcntl_cmd cmd, void *arg) const noexcept {
  return PPOSIX_COMMON_CALL(::fcntl, *fd_, underlying_v(cmd), arg);
}

}