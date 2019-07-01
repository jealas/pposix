#pragma once

#include <system_error>

#include <unistd.h>

#include "pposix/util/underlying_value.hpp"

namespace pposix {

static_assert(std::is_same_v<std::decay_t<decltype(errno)>, int>);
using errno_t = int;

inline errno_t get_errno() noexcept { return errno; }
inline void set_errno(errno_t error_number) noexcept { errno = error_number; }

class errno_context {
 public:
  ~errno_context() { restore(); }
  errno_context() noexcept : saved_errno_{get_errno()} { set_errno(0); }

  void restore() const noexcept { set_errno(saved_errno_); }
  errno_t saved_errno() const noexcept { return saved_errno_; }

 private:
  const errno_t saved_errno_{};
};

inline std::error_code current_errno_code() noexcept {
  return {get_errno(), std::system_category()};
}

inline std::error_code make_errno_code(std::errc err) noexcept {
  return {util::underlying_value(err), std::system_category()};
}

}  // namespace pposix
