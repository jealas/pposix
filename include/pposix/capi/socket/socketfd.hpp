#pragma once

#include "pposix/capi/rawfd.hpp"

namespace pposix::capi::socket {

struct socketfd : rawfd {
  using rawfd::rawfd;
};

}  // namespace pposix::capi::socket
