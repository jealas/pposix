#pragma once

#include <fcntl.h>

#include "pposix/capi/file/flags.hpp"
#include "pposix/capi/file/mode.hpp"
#include "pposix/capi/file/permission.hpp"
#include "pposix/util/underlying_value.hpp"

namespace pposix::capi::file {

rawfd open(char const* native_path, mode m, flags f, permission p) noexcept {
  return {::open(native_path, util::underlying_value(m) | util::underlying_value(f),
                 util::underlying_value(p))};
}

}  // namespace pposix::capi::file
