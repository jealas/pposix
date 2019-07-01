#pragma once

#include "pposix/capi/raw_fd.hpp"

namespace pposix::capi::dirent {

struct dir_fd : rawfd {
  using rawfd::rawfd;
};

}