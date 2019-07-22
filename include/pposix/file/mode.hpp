#pragma once

#include <fcntl.h>

namespace pposix::file {

enum class mode : unsigned { read = O_RDONLY, write = O_WRONLY, read_write = O_RDWR };

}
