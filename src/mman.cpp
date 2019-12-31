#include "pposix/mman.hpp"

#include "pposix/errno.hpp"
#include "pposix/util.hpp"

namespace pposix {

namespace capi {

result<mmap_d> mmap_map(void* addr, size_t len, capi::mmap_protection prot, capi::mmap_flag flags,
                        raw_fd fildes, off_t off) noexcept {
  if (void* ptr{::mmap(addr, len, underlying_v(prot), underlying_v(flags), fildes, off)};
      ptr == MAP_FAILED) {
    return current_errno_code();
  } else {
    return mmap_d{ptr, len};
  }
}

std::error_code mmap_protect(mmap_d descriptor, capi::mmap_protection prot) noexcept {
  return PPOSIX_COMMON_CALL(::mprotect, descriptor.address(), descriptor.length(),
                            underlying_v(prot));
}

}  // namespace capi

mmap::mmap(const mmap_d d) noexcept : mmap_d_{d} {}

std::error_code mmap::unmap() noexcept {
  return PPOSIX_COMMON_CALL(::munmap, mmap_d_->address(), mmap_d_->length());
}

result<shm> shm::unsafe_open(char const* const) noexcept {
  //  return PPOSIX_COMMON_CALL(::shm_open, name);
  return std::error_code{};
}

}