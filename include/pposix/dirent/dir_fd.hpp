#pragma once

#include "pposix/fd.hpp"
#include "pposix/fd.hpp"

namespace pposix::dirent {

struct dir_fd_tag {};

using dir_fd = fd<dir_fd_tag>;

}