#pragma once

#include <system_error>

#include <unistd.h>

#include "pposix/util/underlying_value.hpp"

namespace pposix {

inline std::errc get_errno() noexcept { return std::errc{errno}; }
inline void set_errno(std::errc err) noexcept { errno = util::underlying_value(err); }

class errno_context {
 public:
  ~errno_context() { restore(); }
  errno_context() noexcept : saved_errno_{get_errno()} { set_errno(std::errc{0}); }

  void restore() const noexcept { set_errno(saved_errno_); }
  std::errc saved_errno() const noexcept { return saved_errno_; }

 private:
  const std::errc saved_errno_{};
};

inline std::error_code make_errno_code(std::errc err) noexcept {
  return {util::underlying_value(err), std::system_category()};
}

inline std::error_code current_errno_code() noexcept { return make_errno_code(get_errno()); }

}  // namespace pposix
