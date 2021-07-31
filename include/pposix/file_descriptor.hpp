#pragma once

#include <system_error>
#include <type_traits>
#include <utility>

#include "pposix/errno.hpp"
#include "pposix/unique_descriptor.hpp"

namespace pposix {

using raw_fd_t = int;

struct raw_fd {
 raw_fd_t fd{};

 constexpr explicit operator raw_fd_t() const noexcept
 {
   return fd;
 }
};

std::error_code close_fd(raw_fd fd) noexcept;

using unique_fd =
    unique_descriptor<raw_fd, std::integral_constant<raw_fd, raw_fd{static_cast<raw_fd_t>(-1)}>, close_fd>;

}  // namespace pposix
