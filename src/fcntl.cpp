#include "pposix/fcntl.hpp"

#include "pposix/fd.hpp"
#include "pposix/result.hpp"
#include "pposix/stat.hpp"
#include "pposix/util.hpp"

namespace pposix::capi {

std::error_code fcntl(const raw_fd fd, const capi::fcntl_cmd cmd) noexcept {
  return PPOSIX_COMMON_CALL(::fcntl, static_cast<raw_fd_t>(fd), underlying_v(cmd));
}

std::error_code fcntl(const raw_fd fd, const capi::fcntl_cmd cmd, const int arg) noexcept {
  return PPOSIX_COMMON_CALL(::fcntl, static_cast<raw_fd_t>(fd), underlying_v(cmd), arg);
}

std::error_code fcntl(const raw_fd fd, const capi::fcntl_cmd cmd, void *arg) noexcept {
  return PPOSIX_COMMON_CALL(::fcntl, static_cast<raw_fd_t>(fd), underlying_v(cmd), arg);
}

result<fd> open(const char *path, const capi::access_mode mode, const capi::open_flag flags) noexcept {
  PPOSIX_COMMON_RESULT_DOUBLE_WRAP_IMPL(fd, raw_fd, ::open, path, underlying_v(mode) | underlying_v(flags))
}

result<fd> open(const char *path, capi::access_mode mode, capi::open_flag flags, capi::permission permission) noexcept {
  PPOSIX_COMMON_RESULT_DOUBLE_WRAP_IMPL(
      fd, raw_fd, ::open, path, underlying_v(mode) | underlying_v(flags), underlying_v(permission))
}

}  // namespace pposix::capi
