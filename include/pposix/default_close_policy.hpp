#pragma once

#include <system_error>

#include <unistd.h>

#include "pposix/capi/raw_fd.hpp"
#include "pposix/errno.hpp"
#include "pposix/util/underlying_value.hpp"

namespace pposix {

struct default_close_policy {
  std::error_code operator()(capi::raw_fd fd) const noexcept {
    if (const auto error = ::close(fd.fd()); error == -1) {
      return current_errno_code();
    }

    return {};
  }
};

}  // namespace pposix
