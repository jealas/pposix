#include "pposix/default_close_policy.hpp"

#include <system_error>

#include <unistd.h>

#include "pposix/errno.hpp"
#include "pposix/fd.hpp"
#include "pposix/util.hpp"

namespace pposix {

std::error_code default_close_policy::operator()(raw_fd fd) const noexcept {
  if (const auto error = ::close(fd.raw()); error == -1) {
    return current_errno_code();
  }

  return {};
}

}  // namespace pposix
