#pragma once

#include <system_error>

namespace pposix {

std::errc get_errno() noexcept;
void set_errno(std::errc err) noexcept;

std::error_code make_errno_code(std::errc err) noexcept;

std::error_code current_errno_code() noexcept;

class errno_context {
 public:
  ~errno_context();
  errno_context() noexcept;

  void restore() const noexcept;
  std::errc saved_errno() const noexcept;

 private:
  const std::errc saved_errno_{};
};

}  // namespace pposix
