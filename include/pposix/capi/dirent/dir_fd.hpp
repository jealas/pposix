#pragma once

#include "pposix/capi/rawfd.hpp"

namespace pposix::capi::dirent {

struct dir_fd : rawfd {
  using rawfd::rawfd;
};

}