#pragma once

#include <fcntl.h>

#include "pposix/errno.hpp"
#include "pposix/fd.hpp"
#include "pposix/file/flags.hpp"
#include "pposix/file/mode.hpp"
#include "pposix/file/permission.hpp"
#include "pposix/null_fd.hpp"
#include "pposix/result.hpp"
#include "pposix/unique_fd.hpp"
#include "pposix/util/underlying_value.hpp"

namespace pposix::file {

result<unique_fd<raw_fd>> open(char const* native_path, mode m, flags f, permission p) noexcept {
  const raw_fd open_fd{::open(native_path, util::underlying_value(m) | util::underlying_value(f),
                              util::underlying_value(p))};
  if (open_fd == nullfd) {
    return current_errno_code();
  } else {
    return unique_fd<raw_fd>{open_fd};
  }
}

}  // namespace pposix::file
