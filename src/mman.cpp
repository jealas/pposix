#include "pposix/mman.hpp"

#include "pposix/errno.hpp"
#include "pposix/util.hpp"

namespace pposix {

mmap::mmap(const pposix::mmap_d d) noexcept : mmap_d_{d} {}

result<mmap> mmap::unsafe_map(void* addr, size_t len, capi::mmap_protection prot,
                              capi::mmap_flag flags, raw_fd fildes, off_t off) noexcept {
  if (void* ptr{::mmap(addr, len, underlying_v(prot), underlying_v(flags), fildes, off)};
      ptr == MAP_FAILED) {
    return current_errno_code();
  } else {
    return mmap{mmap_d{ptr, len}};
  }
}

std::error_code mmap::unsafe_protect(capi::mmap_protection prot) noexcept {
  return PPOSIX_COMMON_CALL(::mprotect, mmap_d_->address(), mmap_d_->length(), underlying_v(prot));
}

std::error_code mmap::unmap() noexcept {
  return PPOSIX_COMMON_CALL(::munmap, mmap_d_->address(), mmap_d_->length());
}

}