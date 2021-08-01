#pragma once

#include <sys/mman.h>

#include <cstddef>
#include <system_error>

#include "pposix/descriptor.hpp"
#include "pposix/fd.hpp"
#include "pposix/result.hpp"
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
  inline mmap_d operator()() const noexcept { return {}; }
};

struct mmap_default_close_policy {
  inline std::error_code operator()(mmap_d raw_descriptor) const noexcept {
    return PPOSIX_COMMON_CALL(::munmap, raw_descriptor.address(), raw_descriptor.length());
  }
};

}  // namespace detail

std::error_code close_mmap(const mmap_d &) noexcept;

struct unique_mmap_d : descriptor<mmap_d, detail::get_mmap_null_d, close_mmap> {
  using descriptor::descriptor;
};
namespace capi {

enum class mmap_protection : int {
  read = PROT_READ,
  write = PROT_WRITE,
  exec = PROT_EXEC,
  none = PROT_NONE
};

enum class mmap_flag : int { fixed = MAP_FIXED, private_ = MAP_PRIVATE, shared = MAP_SHARED };

result<mmap_d> mmap_map(void *addr, size_t len, capi::mmap_protection prot, capi::mmap_flag flags,
                        raw_fd fildes, off_t off) noexcept;

std::error_code mmap_protect(capi::mmap_protection prot) noexcept;

}  // namespace capi

class mmap {
 public:
  mmap() noexcept = default;

  explicit mmap(mmap_d d) noexcept;

  mmap(const mmap &) noexcept = delete;
  mmap(mmap &&) noexcept = default;

  mmap &operator=(const mmap &) noexcept = delete;
  mmap &operator=(mmap &&) noexcept = default;

  std::error_code unmap() noexcept;

 private:
  unique_mmap_d mmap_d_{};
};

class shm {
 public:
  shm() noexcept = default;

  shm(const shm &) noexcept = delete;
  shm(shm &&) noexcept = default;

  shm &operator=(const shm &) noexcept = delete;
  shm &operator=(shm &&) noexcept = default;

  static result<shm> unsafe_open(const char *name) noexcept;

 private:
  struct tag {};
  fd shm_fd_{};
};

}  // namespace pposix
