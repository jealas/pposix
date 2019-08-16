#pragma once

#include <sys/stat.h>

namespace pposix {

enum class file_type : ::mode_t {
  block = S_IFBLK,
  character = S_IFCHR,
  fifo = S_IFIFO,
  regular = S_IFREG,
  symlink = S_IFLNK,
  socket = S_IFSOCK
};

}
