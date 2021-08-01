#pragma once

#include "pposix/fd.hpp"
#include "pposix/result.hpp"

namespace pposix::capi {

result<fd> dup(raw_fd) noexcept;
result<dir_fd> dup(raw_dir_fd) noexcept;

}  // namespace pposix::capi