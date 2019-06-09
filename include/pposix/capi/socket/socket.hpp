#pragma once

#include <sys/socket.h>

#include "pposix/capi/socket/domain.hpp"
#include "pposix/capi/socket/flag.hpp"
#include "pposix/capi/socket/protocol.hpp"
#include "pposix/capi/socket/socketfd.hpp"
#include "pposix/capi/socket/type.hpp"
#include "pposix/util/underlying_value.hpp"

namespace pposix::capi::socket {

socketfd socket(domain dom, type typ, flag flags, protocol prot) noexcept {
  return socketfd{::socket(util::underlying_value(dom),
                           util::underlying_value(typ) | util::underlying_value(flags),
                           util::underlying_value(prot))};
}

}