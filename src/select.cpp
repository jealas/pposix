#include "pposix/select.hpp"

#include <sys/select.h>

#include "pposix/errno.hpp"

namespace pposix {

result<int> select(int num_fds, fd_set *read_fds, fd_set *write_fds, fd_set *error_fds,
                   pposix::timeval &timeout) noexcept {
  const auto res{::select(num_fds, read_fds, write_fds, error_fds, &timeout)};
  if (res == -1) {
    return current_errno_code();
  } else {
    return res;
  }
}

result<int> select(int num_fds, fd_set *read_fds, fd_set *write_fds, fd_set *error_fds,
                   decltype(select_no_timeout)) noexcept {
  const auto res{::select(num_fds, read_fds, write_fds, error_fds, nullptr)};
  if (res == -1) {
    return current_errno_code();
  } else {
    return res;
  }
}

result<int> select(int num_fds, fd_set *read_fds, fd_set *write_fds, fd_set *error_fds,
                   decltype(select_poll)) noexcept {
  pposix::timeval zero_timeout{};
  return pposix::select(num_fds, read_fds, write_fds, error_fds, zero_timeout);
}

}  // namespace pposix
