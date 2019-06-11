#pragma once

#include <fcntl.h>

#include "pposix/util/underlying_value.hpp"

namespace pposix::capi::file {

enum class permission : unsigned {
  none = 0u,

  user_read = S_IRUSR,
  user_write = S_IWUSR,
  user_execute = S_IXUSR,
  user_all = S_IRWXU,

  group_read = S_IRGRP,
  group_write = S_IWGRP,
  group_execute = S_IXGRP,
  group_all = S_IRWXG,

  other_read = S_IROTH,
  other_write = S_IWOTH,
  other_execute = S_IXOTH,
  other_all = S_IRWXO,
};

constexpr permission operator|(permission lhs, permission rhs) noexcept {
  return permission{util::underlying_value(lhs) | util::underlying_value(rhs)};
}

constexpr permission &operator|=(permission &lhs, permission rhs) noexcept {
  lhs = lhs | rhs;
  return lhs;
}


}  // namespace pposix::capi::file
