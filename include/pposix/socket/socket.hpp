#pragma once

#include <sys/socket.h>

#include "pposix/socket/domain.hpp"
#include "pposix/socket/flag.hpp"
#include "pposix/socket/protocol.hpp"
#include "pposix/socket/socket_fd.hpp"
#include "pposix/socket/type.hpp"
#include "pposix/util/underlying_value.hpp"

namespace pposix::socket {

socket_fd socket(domain dom, type typ, flag flags, protocol prot) noexcept {
  return socket_fd{::socket(util::underlying_value(dom),
                           util::underlying_value(typ) | util::underlying_value(flags),
                           util::underlying_value(prot))};
}

}