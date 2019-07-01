#pragma once

#include "pposix/capi/raw_fd.hpp"

namespace pposix::capi::socket {

struct socket_fd : raw_fd {
  using raw_fd::raw_fd;
};

}  // namespace pposix::capi::socket
