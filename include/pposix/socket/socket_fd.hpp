#pragma once

#include "pposix/fd.hpp"

namespace pposix::socket {

struct socket_fd_tag {};

using socket_fd = fd<socket_fd_tag>;

}
