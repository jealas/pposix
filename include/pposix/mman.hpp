#pragma once

#include <cstddef>
#include <system_error>

#include <sys/mman.h>

#include "pposix/result.hpp"
#include "pposix/unique_d.hpp"
#include "pposix/unique_fd.hpp"
#include "pposix/util.hpp"

namespace pposix {

class mmap_d {
 public:
  mmap_d() noexcept = default;
  constexpr mmap_d(void *address, size_t length) : address_{address}, length_{length} {}

  constexpr void *address() noexcept { return address_; }
  constexpr const void *address() const noexcept { return address_; }

  constexpr size_t length() const noexcept { return length_; }

 private:
  void *address_{MAP_FAILED};
  size_t length_{0u};
};

bool operator==(const mmap_d &lhs, const mmap_d &rhs) noexcept {
  return lhs.address() == rhs.address() and lhs.length() == rhs.length();
}

namespace detail {

struct get_mmap_null_d {
  inline mmap_d operator()() const noexcept { return {MAP_FAILED, 0u}; }
};

struct mmap_default_close_policy {
  std::error_code operator()(mmap_d raw_descriptor) const noexcept {
    return ::munmap(raw_descriptor.address(), raw_descriptor.length()) == -1 ? current_errno_code()
                                                                             : std::error_code{};
  }
};

}  // namespace detail

namespace capi {

enum class mmap_protection : int {
  read = PROT_READ,
  write = PROT_WRITE,
  exec = PROT_EXEC,
  none = PROT_NONE
};

enum class mmap_flag : int { fixed = MAP_FIXED, private_ = MAP_PRIVATE, shared = MAP_SHARED };

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
inline constexpr mmap_flag<capi::mmap_flag::private_> mmap_private{};
inline constexpr mmap_flag<capi::mmap_flag::shared> mmap_shared{};

class mmap {
 public:
  mmap() noexcept = default;

  explicit mmap(mmap_d d) noexcept;

  mmap(const mmap &) noexcept = delete;
  mmap(mmap &&) noexcept = default;

  mmap &operator=(const mmap &) noexcept = delete;
  mmap &operator=(mmap &&) noexcept = default;

  static result<mmap> unsafe_map(void *addr, size_t len, capi::mmap_protection prot,
                                 capi::mmap_flag flags, raw_fd fildes, off_t off) noexcept;

  std::error_code unsafe_protect(capi::mmap_protection prot) noexcept;

  template <capi::mmap_protection ProtectionFlags, capi::mmap_flag Flags>
  static result<mmap> map(void *addr, size_t len, mmap_protection_set<ProtectionFlags>,
                          mmap_flag_set<Flags>, raw_fd fildes, off_t off) noexcept {
    static_assert(
        not(mmap_protection_set<ProtectionFlags>::has(mmap_no_access) and
            (mmap_protection_set<ProtectionFlags>::has(mmap_read) or
             mmap_protection_set<ProtectionFlags>::has(mmap_write) or
             mmap_protection_set<ProtectionFlags>::has(mmap_execute))),
        "pposix::mmap_no_access cannot be set with pposix::mmap_read, pposix::mmap_write "
        "or pposix::mmap_execute.");

    static_assert(
        not(mmap_flag_set<Flags>::has(mmap_private) and mmap_flag_set<Flags>::has(mmap_shared)),
        "You can only specify one of pposix::mmap_private or pposix::mmap_shared, not both.");

    return unsafe_map(addr, len, ProtectionFlags, Flags, fildes, off);
  }

  std::error_code unmap() noexcept;

  template <capi::mmap_protection ProtectionFlags>
  std::error_code protect(mmap_protection_set<ProtectionFlags>) noexcept {
    static_assert(not(mmap_protection_set<ProtectionFlags>::has(mmap_no_access) and
                      (mmap_protection_set<ProtectionFlags>::has(mmap_read) or
                       mmap_protection_set<ProtectionFlags>::has(mmap_write))),
                  "pposix::mmap_no_access cannot be set with pposix::mmap_read or "
                  "pposix::mmap_write.");

    return unsafe_protect(ProtectionFlags);
  }

 private:
  struct mmap_d_tag {};

  using unique_mmap_d =
      unique_d<mmap_d_tag, mmap_d, detail::get_mmap_null_d, detail::mmap_default_close_policy>;

  unique_mmap_d mmap_d_{};
};

}  // namespace pposix
