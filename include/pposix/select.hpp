#pragma once

#include <sys/select.h>

#include "pposix/fd.hpp"
#include "pposix/result.hpp"
#include "pposix/time.hpp"

namespace pposix {

class fd_set : public ::fd_set {
 public:
  inline fd_set() noexcept { FD_ZERO(this); }

  inline void clear() noexcept { FD_ZERO(this); }

  inline void set(raw_fd fd) noexcept { FD_SET(fd.raw(), this); }
  inline void unset(raw_fd fd) noexcept { FD_CLR(fd.raw(), this); }
  inline bool is_set(raw_fd fd) const noexcept { return FD_ISSET(fd.raw(), this); }

  static constexpr int max_fd() noexcept { return FD_SETSIZE; }
};

result<int> select(int num_fds, fd_set *read_fds, fd_set *write_fds, fd_set *error_fds,
                   pposix::timeval &timeout) noexcept;

struct {
} select_no_timeout;

result<int> select(int num_fds, fd_set *read_fds, fd_set *write_fds, fd_set *error_fds,
                   decltype(select_no_timeout)) noexcept;

struct {
} select_poll;

result<int> select(int num_fds, fd_set *read_fds, fd_set *write_fds, fd_set *error_fds,
                   decltype(select_poll)) noexcept;

}  // namespace pposix
