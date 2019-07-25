#include "pposix/errno.hpp"

#include <unistd.h>

#include "pposix/util/underlying_value.hpp"

namespace pposix {

std::errc get_errno() noexcept { return std::errc{errno}; }
void set_errno(std::errc err) noexcept { errno = util::underlying_value(err); }

std::error_code make_errno_code(std::errc err) noexcept {
  return {util::underlying_value(err), std::system_category()};
}

std::error_code current_errno_code() noexcept { return make_errno_code(get_errno()); }

errno_context::~errno_context() { restore(); }
errno_context::errno_context() noexcept : saved_errno_{get_errno()} { set_errno(std::errc{0}); }

void errno_context::restore() const noexcept { set_errno(saved_errno_); }
std::errc errno_context::saved_errno() const noexcept { return saved_errno_; }

}  // namespace pposix
