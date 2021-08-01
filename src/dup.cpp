#include "pposix/dup.hpp"

#include <unistd.h>

#include "pposix/util.hpp"

namespace pposix::capi {

result<fd> dup(const raw_fd fd) noexcept {
  PPOSIX_COMMON_RESULT_DOUBLE_WRAP_IMPL(::pposix::fd, raw_fd, ::dup, static_cast<raw_fd_t>(fd))
}

}  // namespace pposix::capi