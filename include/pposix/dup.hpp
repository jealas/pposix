#pragma once

#include "pposix/fd.hpp"
#include "pposix/result.hpp"

namespace pposix::capi {

result<fd> dup(raw_fd fd) noexcept;

}