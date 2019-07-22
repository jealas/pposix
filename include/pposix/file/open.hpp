#pragma once

#include <fcntl.h>

#include "pposix/file/flags.hpp"
#include "pposix/file/mode.hpp"
#include "pposix/file/permission.hpp"
#include "pposix/util/underlying_value.hpp"

namespace pposix::file {

rawfd open(char const* native_path, mode m, flags f, permission p) noexcept {
  return {::open(native_path, util::underlying_value(m) | util::underlying_value(f),
                 util::underlying_value(p))};
}

}
