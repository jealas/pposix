#pragma once

#include "pposix/capi/raw_fd.hpp"
#include "pposix/fd.hpp"

namespace pposix::capi::dirent {

struct dir_fd_tag {};

using dir_fd = fd<dir_fd_tag>;

}