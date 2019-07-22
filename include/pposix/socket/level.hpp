#pragma once

#include <sys/socket.h>

namespace pposix::socket {

enum class level : int {
  socket = SOL_SOCKET,
  ip = IPPROTO_IP,
  ipv6 = IPPROTO_IPV6,
  icmp = IPPROTO_ICMP,
  raw = IPPROTO_RAW,
  tcp = IPPROTO_TCP,
  udp = IPPROTO_UDP,
};

}