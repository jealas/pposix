#include "pposix/fd.hpp"

#include <unistd.h>

#include "pposix/util.hpp"

namespace pposix {

std::error_code close_raw_fd(raw_fd fd) noexcept {
  return PPOSIX_COMMON_CALL(::close, static_cast<raw_fd_t>(fd));
}

}  // namespace pposix
