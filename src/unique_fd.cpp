#include "pposix/unique_fd.hpp"

#include <unistd.h>

#include "pposix/util.hpp"

namespace pposix {

std::error_code close_fd(const raw_fd fd) noexcept {
  return PPOSIX_COMMON_CALL(::close, static_cast<raw_fd_t>(fd));
}

}  // namespace pposix
