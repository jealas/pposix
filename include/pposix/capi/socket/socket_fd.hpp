#pragma once

#include "pposix/raw_fd.hpp"

namespace pposix::capi::socket {

struct socket_fd : raw_fd {
  using raw_fd::raw_fd;
};

}  // namespace pposix::capi::socket
