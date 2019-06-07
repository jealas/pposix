#pragma once

#include <unistd.h>

namespace pposix::file {

using offset_t = off_t;

constexpr bool is_error(offset_t offset) { return offset == offset_t{-1}; }

}  // namespace pposix::file
