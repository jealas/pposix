#pragma once

#include <system_error>

#include "pposix/fd.hpp"

namespace pposix {

struct default_close_policy {
  std::error_code operator()(raw_fd fd) const noexcept;
};

}  // namespace pposix
