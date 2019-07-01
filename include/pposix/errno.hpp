#pragma once

#include <cerrno>

namespace pposix {

static_assert(std::is_same_v<std::decay_t<decltype(errno)>, int>);
using errno_t = int;

errno_t get_errno() noexcept { return errno; }
void set_errno(errno_t error_number) noexcept { errno = error_number; }

class errno_context {
 public:
  ~errno_context() { restore(); }
  errno_context() noexcept : saved_errno_{get_errno()} { set_errno(0); }

  void restore() const noexcept { set_errno(saved_errno_); }
  errno_t saved_errno() const noexcept { return saved_errno_; }

 private:
  const errno_t saved_errno_{};
};

}  // namespace pposix
