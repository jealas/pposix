#pragma once

#include <system_error>

#include <unistd.h>

#include "pposix/util/underlying_value.hpp"

namespace pposix {

inline std::error_code errno_code() noexcept { return {errno, std::system_category()}; }

inline std::error_code make_errno_code(std::errc err) noexcept {
  return {util::underlying_value(err), std::system_category()};
}

}  // namespace pposix
