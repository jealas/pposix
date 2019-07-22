#pragma once

#include <system_error>

#include <unistd.h>

#include "pposix/errno.hpp"
#include "pposix/fd.hpp"
#include "pposix/util/underlying_value.hpp"

namespace pposix {

struct default_close_policy {
  std::error_code operator()(raw_fd fd) const noexcept {
    if (const auto error = ::close(fd.raw()); error == -1) {
      return current_errno_code();
    }

    return {};
  }
};

}  // namespace pposix
