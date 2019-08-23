#pragma once

#include <cstddef>
#include <memory>
#include <system_error>

#include <sys/mman.h>

#include "pposix/descriptor.hpp"
#include "pposix/resource.hpp"
#include "pposix/result.hpp"
#include "pposix/unique_descriptor.hpp"
#include "pposix/unique_fd.hpp"

namespace pposix {

class mmap_descriptor {
 public:
  constexpr mmap_descriptor() noexcept = default;
  constexpr mmap_descriptor(void *address, size_t length) : address_{address}, length_{length} {}

  constexpr void *address() noexcept { return address_; }
  constexpr const void *address() const noexcept { return address_; }

  constexpr size_t length() const noexcept { return length_; }

 private:
  void *address_{MAP_FAILED};
  size_t length_{0u};
};

bool operator==(const mmap_descriptor &lhs, const mmap_descriptor &rhs) noexcept {
  return lhs.address() == rhs.address() and lhs.length() == rhs.length();
}

namespace detail {

struct mmap_descriptor_tag {};

struct mmap_null_descriptor {
  inline mmap_descriptor operator()() const noexcept { return {MAP_FAILED, 0u}; }
};

}  // namespace detail

using mmap_d =
    descriptor<detail::mmap_descriptor_tag, mmap_descriptor, detail::mmap_null_descriptor>;

struct mmap_default_close_policy {
  std::error_code operator()(mmap_d raw_descriptor) const noexcept {
    return ::munmap(raw_descriptor.raw().address(), raw_descriptor.raw().length()) == -1
               ? current_errno_code()
               : std::error_code{};
  }
};

using unique_mmap_d = unique_descriptor<mmap_d, mmap_default_close_policy>;

namespace capi {

enum class mmap_protection : int {
  read = PROT_READ,
  write = PROT_WRITE,
  exec = PROT_EXEC,
  none = PROT_NONE
};

enum class mmap_flag : int { fixed = MAP_FIXED, private_ = MAP_PRIVATE, shared = MAP_SHARED };

result<unique_mmap_d> mmap(void *addr, size_t len, mmap_protection prot, mmap_flag flags,
                           raw_fd fildes, off_t off) noexcept;

std::error_code munmap(mmap_d resource, size_t length) noexcept;

std::error_code mprotect(mmap_d resource, size_t length, mmap_protection prot) noexcept;

}  // namespace capi

}  // namespace pposix
