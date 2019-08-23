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
#include "pposix/util.hpp"

namespace pposix {

class mmap_descriptor {
 public:
  inline mmap_descriptor() noexcept = default;
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

std::error_code munmap(void *resource, size_t length) noexcept;

std::error_code mprotect(void *resource, size_t length, mmap_protection prot) noexcept;

}  // namespace capi

template <capi::mmap_flag Flag>
using mmap_flag = enum_flag<capi::mmap_flag, Flag>;

template <capi::mmap_flag Flags>
using mmap_flag_set = enum_flag_set<capi::mmap_flag, Flags>;

template <capi::mmap_protection Flag>
using mmap_protection = enum_flag<capi::mmap_protection, Flag>;

template <capi::mmap_protection Flags>
using mmap_protection_set = enum_flag<capi::mmap_protection, Flags>;

inline constexpr mmap_protection<capi::mmap_protection::none> mmap_no_access{};
inline constexpr mmap_protection<capi::mmap_protection::read> mmap_read{};
inline constexpr mmap_protection<capi::mmap_protection::write> mmap_write{};
inline constexpr mmap_protection<capi::mmap_protection::exec> mmap_execute{};

inline constexpr mmap_flag<capi::mmap_flag::fixed> mmap_fixed{};
inline constexpr mmap_flag<capi::mmap_flag::private_> mmap_private_{};
inline constexpr mmap_flag<capi::mmap_flag::shared> mmap_shared{};

template <capi::mmap_protection ProtectionFlags, capi::mmap_flag Flags>
result<unique_mmap_d> mmap(void *addr, size_t len, mmap_protection_set<ProtectionFlags>,
                           mmap_flag_set<Flags>, raw_fd fildes, off_t off) noexcept {
  static_assert(not(mmap_protection_set<ProtectionFlags>::has(mmap_no_access) and
                    (mmap_protection_set<ProtectionFlags>::has(mmap_read) or
                     mmap_protection_set<ProtectionFlags>::has(mmap_write) or
                     mmap_protection_set<ProtectionFlags>::has(mmap_execute))),
                "pposix::mmap_no_access cannot be set with pposix::mmap_read, pposix::mmap_write "
                "or pposix::mmap_execute.");

  static_assert(
      not(mmap_flag_set<Flags>::has(mmap_private_) and mmap_flag_set<Flags>::has(mmap_shared)),
      "You can only specify one of pposix::mmap_private_ or pposix::mmap_shared, not both.");

  return capi::mmap(addr, len, ProtectionFlags, Flags, fildes, off);
}

std::error_code munmap(mmap_descriptor resource) noexcept;

template <capi::mmap_protection ProtectionFlags>
std::error_code mprotect(mmap_descriptor resource, mmap_protection_set<ProtectionFlags>) noexcept {
  static_assert(not(mmap_protection_set<ProtectionFlags>::has(mmap_no_access) and
                    (mmap_protection_set<ProtectionFlags>::has(mmap_read) or
                     mmap_protection_set<ProtectionFlags>::has(mmap_write))),
                "pposix::mmap_no_access cannot be set with pposix::mmap_read or "
                "pposix::mmap_write.");

  return capi::mprotect(resource.address(), resource.length(), ProtectionFlags);
}

}  // namespace pposix
