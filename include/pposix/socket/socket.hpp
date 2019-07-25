#pragma once

#include <sys/socket.h>

#include "pposix/errno.hpp"
#include "pposix/result.hpp"
#include "pposix/socket/domain.hpp"
#include "pposix/socket/flag.hpp"
#include "pposix/socket/protocol.hpp"
#include "pposix/socket/socket_fd.hpp"
#include "pposix/socket/type.hpp"
#include "pposix/unique_fd.hpp"
#include "pposix/util/underlying_value.hpp"

namespace pposix::socket {

result<unique_fd<socket_fd>> socket(domain dom, type typ, flag flags, protocol prot) noexcept {
  const socket_fd sock_fd{::socket(util::underlying_value(dom),
                                   util::underlying_value(typ) | util::underlying_value(flags),
                                   util::underlying_value(prot))};
  if (sock_fd == nullfd) {
    return current_errno_code();
  } else {
    return unique_fd<socket_fd>{sock_fd};
  }
}

}