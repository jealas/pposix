#pragma once

#include <system_error>
#include <type_traits>
#include <utility>

#include "pposix/descriptor.hpp"
#include "pposix/errno.hpp"

namespace pposix {

using raw_fd_t = int;

class raw_fd {
 public:
  constexpr explicit raw_fd(const raw_fd_t fd) noexcept : fd_{fd} {}

  constexpr explicit operator raw_fd_t() const noexcept { return fd_; }

 private:
  raw_fd_t fd_{};
};

std::error_code close_raw_fd(raw_fd fd) noexcept;

using fd =
    descriptor<raw_fd, descriptor_constant<raw_fd, raw_fd_t, -1>, close_raw_fd>;

struct raw_dir_fd : raw_fd {
  using raw_fd::raw_fd;
};

using raw_dir_fd_t = raw_fd_t;

using dir_fd =
    descriptor<raw_dir_fd, descriptor_constant<raw_dir_fd, raw_dir_fd_t, -1>, close_raw_fd>;

}  // namespace pposix
