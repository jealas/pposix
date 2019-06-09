#pragma once

#include <fcntl.h>

namespace pposix::capi::file {

enum class mode : unsigned { read = O_RDONLY, write = O_WRONLY, read_write = O_RDWR };

}
