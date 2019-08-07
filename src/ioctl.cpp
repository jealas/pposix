#include "pposix/ioctl.hpp"

#include <stdexcept>
#include <string>

#include <sys/ioctl.h>

#include "pposix/errno.hpp"
#include "pposix/util.hpp"

namespace pposix {

result<ioctl_int> ioctl(raw_fd fd, ioctl_request r, int i) noexcept {
  if (const auto res{::ioctl(fd.raw(), underlying_value(r), i)}; res == -1) {
    return current_errno_code();
  } else {
    return res;
  }
}

result<ioctl_int> ioctl(raw_fd fd, ioctl_request r, void *ptr) noexcept {
  if (const auto res{::ioctl(fd.raw(), underlying_value(r), ptr)}; res == -1) {
    return current_errno_code();
  } else {
    return res;
  }
}

result<ioctl_int> ioctl(raw_fd fd, ioctl_request r, const void *ptr) noexcept {
  if (const auto res{::ioctl(fd.raw(), underlying_value(r), ptr)}; res == -1) {
    return current_errno_code();
  } else {
    return res;
  }
}

result<ioctl_int> ioctl(raw_fd fd, ioctl_request r, nullptr_t) noexcept {
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
