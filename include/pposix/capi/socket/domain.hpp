#pragma once

#include <sys/socket.h>

namespace pposix::capi::socket {

enum class domain : unsigned {
  unspec = AF_UNSPEC,
  local = AF_LOCAL,
  unix = AF_UNIX,
  file = AF_FILE,
  inet = AF_INET,
  ax25 = AF_AX25,
  ipx = AF_IPX,
  appletalk = AF_APPLETALK,
  x25 = AF_X25,
  inet6 = AF_INET6,
  decnet = AF_DECnet,
  key = AF_KEY,
  netlink = AF_NETLINK,
  packet = AF_PACKET,
  rds = AF_RDS,
  pppox = AF_PPPOX,
  llc = AF_LLC,
  ib = AF_IB,
  mpls = AF_MPLS,
  can = AF_CAN,
  tipc = AF_TIPC,
  bluetooth = AF_BLUETOOTH,
  alg = AF_ALG,
  vsock = AF_VSOCK,
  kcm = AF_KCM,
  xdp = AF_XDP
};

}
