#pragma once

#include <system_error>
#include <type_traits>
#include <utility>

#include "pposix/errno.hpp"
#include "pposix/unique_descriptor.hpp"

namespace pposix {

using raw_fd_t = int;

class raw_fd {
 public:
  constexpr explicit raw_fd(const raw_fd_t fd) noexcept : fd_{fd} {}

  constexpr explicit operator raw_fd_t() const noexcept { return fd_; }

 private:
  raw_fd_t fd_{};
};

std::error_code close_fd(raw_fd fd) noexcept;

using unique_fd = unique_descriptor<raw_fd, integral_descriptor<raw_fd, raw_fd_t, -1>, close_fd>;

}  // namespace pposix
