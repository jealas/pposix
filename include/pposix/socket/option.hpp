#ifndef PPOSIX_SOCKET_OPTION_HPP
#define PPOSIX_SOCKET_OPTION_HPP

#include <sys/socket.h>

namespace pposix::socket {

enum class option : unsigned {
  acceptconn = SO_ACCEPTCONN,
  broadcast = SO_BROADCAST,
  debug = SO_DEBUG,
  dontroute = SO_DONTROUTE,
  error = SO_ERROR,
  keepalive = SO_KEEPALIVE,
  linger = SO_LINGER,
  oobinline = SO_OOBINLINE,
  rcvbuf = SO_RCVBUF,
  rcvlowat = SO_RCVLOWAT,
  rcvtimeo = SO_RCVTIMEO,
  reuseaddr = SO_REUSEADDR,
  sndbuf = SO_SNDBUF,
  sndlowat = SO_SNDLOWAT,
  sndtimeo = SO_SNDTIMEO,
  type = SO_TYPE
};

}

#endif  // PPOSIX_SOCKET_OPTION_HPP
