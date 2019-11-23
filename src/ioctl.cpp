#include "pposix/ioctl.hpp"

#include <stdexcept>
#include <string>

#include <sys/ioctl.h>
#include <unistd.h>

#include "pposix/errno.hpp"
#include "pposix/util.hpp"

namespace pposix {

result<ioctl_int> ioctl(raw_fd fd, ioctl_request r, int i) noexcept {
  return PPOSIX_COMMON_CALL(::ioctl, fd, underlying_v(r), i);
}

result<ioctl_int> ioctl(raw_fd fd, ioctl_request r, void *ptr) noexcept {
  return PPOSIX_COMMON_CALL(::ioctl, fd, underlying_v(r), ptr);
}

result<ioctl_int> ioctl(raw_fd fd, ioctl_request r, const void *ptr) noexcept {
  return PPOSIX_COMMON_CALL(::ioctl, fd, underlying_v(r), ptr);
}

result<ioctl_int> ioctl(raw_fd fd, ioctl_request r, std::nullptr_t) noexcept {
  void *ptr = nullptr;
  return pposix::ioctl(fd, r, ptr);
}

result<ioctl_int> ioctl(raw_fd fd, ioctl_request r, any_view val) noexcept {
  return pposix::ioctl(fd, r, val.data());
}

result<ioctl_int> ioctl(raw_fd fd, ioctl_request r, any_cview val) noexcept {
  return pposix::ioctl(fd, r, val.data());
}

}  // namespace pposix
