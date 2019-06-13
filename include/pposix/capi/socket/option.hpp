#pragma once

#include <sys/socket.h>

namespace pposix::capi::socket {

enum class option : int {
  debug = SO_DEBUG,
  broadcast = SO_BROADCAST,
  reuseaddr = SO_REUSEADDR,
  keepalive = SO_KEEPALIVE,
  linger = SO_LINGER,
  oobinline = SO_OOBINLINE,
  rcvbuf = SO_RCVBUF,
  sndbuf = SO_SNDBUF,
  dontroute = SO_DONTROUTE,
  rcvlowat = SO_RCVLOWAT,
  rcvtimeo = SO_RCVTIMEO,
  sndlowat = SO_SNDLOWAT,
  sndtimeo = SO_SNDTIMEO,
  acceptconn = SO_ACCEPTCONN,
  error = SO_ERROR,
  type = SO_TYPE
};

}
