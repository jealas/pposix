#pragma once

#include <sys/socket.h>

#include "pposix/capi/socket/socketfd.hpp"

namespace pposix::capi::socket {

socketfd socket(domain d, type t, flag f, protocol p) noexcept {
  return {::socket(util::underlying_value(d),
                   util::underlying_value(t) | util::underlying_value(f), p)};
}

}