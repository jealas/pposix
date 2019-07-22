#pragma once

#include "pposix/fd.hpp"

namespace pposix {

struct raw_tag {};

using raw_fd = fd<raw_tag>;

}  // namespace pposix
