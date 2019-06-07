#pragma once

#include <sys/socket.h>

#include "pposix/capi/socket/socketfd.hpp"
#include "pposix/result.hpp"

namespace pposix::capi::socket {

result<socketfd> socket(const domain domain, const type typ, const flag &flags,
                        const protocol protocol) {
  const socketfd fd{::socket(util::underlying_value(domain),
                             util::underlying_value(typ) | util::underlying_value(flags),
                             protocol)};

  if (fd == posix::nullfd) {
    return posix::errno_code();
  } else {
    return socketfd{fd};
  }
}

}