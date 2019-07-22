#pragma once

#include <system_error>

#include <dirent.h>

#include "pposix/dirent/closedir.hpp"

namespace pposix::dirent {

struct close_policy {
  std::error_code operator()(DIR *dir) { return dirent::closedir(dir); }
};

}