#pragma once

#include <sys/socket.h>

namespace pposix::capi::socket {

enum class flag : unsigned { none = 0u, closexec = SOCK_CLOEXEC, nonblock = SOCK_NONBLOCK };

flag operator|(const flag &lhs, const flag &rhs) noexcept {
  return flag{util::underlying_value(lhs) | util::underlying_value(rhs)};
}
}  // namespace pposix::capi::socket
