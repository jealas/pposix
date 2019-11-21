#include "pposix/mman.hpp"

#include "pposix/errno.hpp"
#include "pposix/util.hpp"

namespace pposix {

namespace capi {

result<unique_mmap_d> mmap(void* addr, size_t len, mmap_protection prot, mmap_flag flags,
                           raw_fd fildes, off_t off) noexcept {
  void* ptr =
      ::mmap(addr, len, underlying_value(prot), underlying_value(flags), fildes.raw(), off);
  if (ptr == MAP_FAILED) {
    return current_errno_code();
  } else {
    return unique_mmap_d{mmap_descriptor{ptr, len}};
  }
}

std::error_code munmap(void* resource, size_t length) noexcept {
  return ::munmap(resource, length) == -1 ? current_errno_code() : std::error_code{};
}

std::error_code mprotect(void* resource, size_t length, mmap_protection prot) noexcept {
  return ::mprotect(resource, length, underlying_value(prot)) == -1 ? current_errno_code()
                                                                    : std::error_code{};
}

}  // namespace capi

std::error_code munmap(mmap_descriptor resource) noexcept {
  return ::munmap(resource.address(), resource.length()) == -1 ? current_errno_code()
                                                               : std::error_code{};
}

}