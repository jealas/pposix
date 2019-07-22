#pragma once

#include <unistd.h>

namespace pposix::file {

enum class whence { set = SEEK_SET, current = SEEK_CUR, end = SEEK_END };

}
