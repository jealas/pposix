#pragma once

#include <fcntl.h>

#include "pposix/util/underlying_value.hpp"

namespace pposix::file {

enum class flags : unsigned {
  append = O_APPEND,
  create = O_CREAT,
  dsync = O_DSYNC,
  excl = O_EXCL,
  noctty = O_NOCTTY,
  nonblock = O_NONBLOCK rsync = O_RSYNC,
  sync = O_SYNC,
  truncate = O_TRUNC,
};

flags operator|(flags lhs, flags rhs) noexcept {
  return flags{util::underlying_value(lhs) | util::underlying_value(rhs)};
}
}  // namespace pposix::file
