#pragma once

#include <system_error>
#include <type_traits>
#include <utility>

#include "pposix/errno.hpp"
#include "pposix/unique_d.hpp"

namespace pposix {

using raw_fd = int;

struct fd_close_policy {
  std::error_code operator()(raw_fd fd) const noexcept;
};

struct fd_tag {};

using unique_fd = unique_d<fd_tag, int, std::integral_constant<int, -1>, fd_close_policy>;

}  // namespace pposix
