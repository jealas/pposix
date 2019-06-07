#pragma once

#include <sys/socket.h>

namespace pposix::capi::socket {

enum class type : unsigned {
  stream = SOCK_STREAM,
  dgram = SOCK_DGRAM,
  seq = SOCK_SEQPACKET,
  raw = SOCK_RAW,
  rdm = SOCK_RDM
};

}
