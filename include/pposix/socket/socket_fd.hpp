#pragma once

#include "pposix/fd.hpp"

namespace pposix::capi::socket {

struct socket_fd_tag {};

using socket_fd = fd<socket_fd_tag>;

}  // namespace pposix::capi::socket
