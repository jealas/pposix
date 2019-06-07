#pragma once

#include <unistd.h>

namespace pposix::file {

enum class seek_whence { set = SEEK_SET, current = SEEK_CUR, end = SEEK_END };

}
