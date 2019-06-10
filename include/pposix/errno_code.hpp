#pragma once

#include <system_error>

#include <unistd.h>

namespace pposix {

inline std::error_code errno_code() { return {errno, std::system_category()}; }

}  // namespace pposix
