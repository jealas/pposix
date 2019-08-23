#include "pposix/mman.hpp"

#include "pposix/errno.hpp"
#include "pposix/util.hpp"

namespace pposix {

namespace capi {

result<unique_mmap_d> mmap(void *addr, size_t len, mmap_protection prot, mmap_flag flags,
                           raw_fd fildes, off_t off) noexcept {
  void *ptr =
      ::mmap(addr, len, underlying_value(prot), underlying_value(flags), fildes.raw(), off);
  if (ptr == MAP_FAILED) {
    return current_errno_code();
  } else {
    return unique_mmap_d{mmap_descriptor{ptr, len}};
  }
}

std::error_code munmap(mmap_d resource) noexcept {
  return ::munmap(resource.raw().address(), resource.raw().length()) == -1 ? current_errno_code()
                                                                           : std::error_code{};
}

std::error_code mprotect(mmap_d resource, mmap_protection prot) noexcept {
  return ::mprotect(resource.raw().address(), resource.raw().length(), underlying_value(prot)) ==
                 -1
             ? current_errno_code()
             : std::error_code{};
}

}  // namespace capi

}