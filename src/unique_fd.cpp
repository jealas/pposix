#include "pposix/unique_fd.hpp"

#include <unistd.h>

namespace pposix {

std::error_code fd_close_policy::operator()(const raw_fd &fd) const noexcept {
  if (const auto error = ::close(fd.raw()); error == -1) {
    return current_errno_code();
  }

  return {};
}

}  // namespace pposix
