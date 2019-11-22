#include "pposix/mman.hpp"

#include "pposix/errno.hpp"
#include "pposix/util.hpp"

namespace pposix {

mmap::mmap(const pposix::mmap_d d) noexcept : mmap_d_{d} {}

result<mmap> mmap::unsafe_map(void* addr, size_t len, capi::mmap_protection prot,
                              capi::mmap_flag flags, raw_fd fildes, off_t off) noexcept {
  void* ptr = ::mmap(addr, len, underlying_value(prot), underlying_value(flags), fildes, off);
  if (ptr == MAP_FAILED) {
    return current_errno_code();
  } else {
    return mmap{mmap_d{ptr, len}};
  }
}

std::error_code mmap::unsafe_protect(capi::mmap_protection prot) noexcept {
  return ::mprotect(mmap_d_->address(), mmap_d_->length(), underlying_value(prot)) == -1
             ? current_errno_code()
             : std::error_code{};
}

std::error_code mmap::unmap() noexcept {
  return ::munmap(mmap_d_->address(), mmap_d_->length()) == -1 ? current_errno_code()
                                                               : std::error_code{};
}

}