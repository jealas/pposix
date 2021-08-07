#include "pposix/directory.hpp"

#include <utility>

#include "pposix/dirent.hpp"

namespace pposix {

directory::directory(dir_fd fd) noexcept : fd_{std::move(fd)} {}

std::error_code directory::close() noexcept { return fd_.close(); }

}  // namespace pposix
