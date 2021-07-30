#pragma once

#include <system_error>
#include <type_traits>
#include <utility>

#include "pposix/errno.hpp"
#include "pposix/unique_d.hpp"

namespace pposix {

using raw_fd_t = int;

enum class raw_fd : raw_fd_t {};

std::error_code close_fd(raw_fd fd) noexcept;

using unique_fd =
    unique_d<raw_fd, std::integral_constant<raw_fd, raw_fd{static_cast<raw_fd_t>(-1)}>, close_fd>;

}  // namespace pposix
