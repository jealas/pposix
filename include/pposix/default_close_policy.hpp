#pragma once

#include <system_error>

#include <unistd.h>

#include "pposix/capi/rawfd.hpp"
#include "pposix/errno_code.hpp"
#include "pposix/util/underlying_value.hpp"

namespace pposix {

struct default_close_policy {
  std::error_code operator()(capi::rawfd fd) const noexcept {
    if (const auto error = ::close(fd.fd()); error == -1) {
      return errno_code();
    }

    return {};
  }
};

}  // namespace pposix
