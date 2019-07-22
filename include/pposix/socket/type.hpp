#pragma once

namespace pposix::socket {

enum class type : int {
  stream = SOCK_STREAM,
  dgram = SOCK_DGRAM,
  seqpacket = SOCK_SEQPACKET,
  raw = SOCK_RAW,
  rdm = SOCK_RDM
};

}
