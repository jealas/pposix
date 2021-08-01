#pragma once

#include <cassert>
#include <system_error>
#include <utility>

#include "pposix/errno.hpp"

namespace pposix {

struct null_d_t {};
inline constexpr null_d_t null_d{};

template <class FdWrapper, class UnderlyingFd, UnderlyingFd Fd>
struct descriptor_constant {
  constexpr FdWrapper operator()() const noexcept { return FdWrapper{Fd}; }
};

template <class Descriptor, class GetNull, auto ClosePolicy>
class [[nodiscard]] descriptor {
  static_assert(noexcept(GetNull{}()));
  static_assert(noexcept(Descriptor{GetNull{}()}));
  static_assert(noexcept(Descriptor{std::declval<Descriptor>()}));
  static_assert(noexcept(ClosePolicy(std::declval<Descriptor>())));

 public:
   descriptor() noexcept = default;

   explicit descriptor(null_d_t) noexcept {}

   explicit descriptor(Descriptor descriptor) : raw_descriptor_{descriptor} {}

  ~descriptor() {
    if (const auto err{close()}) {
      assert(!err);
    }
  }

  descriptor(const descriptor &other) = delete;
   descriptor(descriptor &&other) noexcept { std::swap(raw_descriptor_, other.raw_descriptor_); }

  descriptor &operator=(const descriptor &) = delete;

   descriptor &operator=(descriptor &&other) noexcept {
    std::swap(raw_descriptor_, other.raw_descriptor_);
  }

  [[nodiscard]]  bool empty() const noexcept { return raw_descriptor_ == GetNull{}(); }
   bool operator==(null_d_t) const noexcept { return empty(); }

   explicit operator bool() const noexcept { return not empty(); }

   Descriptor raw() const noexcept { return raw_descriptor_; }
   Descriptor operator*() const noexcept { return raw(); }

   Descriptor *operator->() noexcept { return &raw_descriptor_; }
   Descriptor const *operator->() const noexcept { return &raw_descriptor_; }

  [[nodiscard]] Descriptor release() noexcept {
    Descriptor tmp_fd{raw_descriptor_};
    raw_descriptor_ = GetNull{}();
    return tmp_fd;
  }

  [[nodiscard]] std::error_code close() noexcept {
    if (!empty()) {
      const auto error = ClosePolicy(raw());
      if (not error) {
        raw_descriptor_ = GetNull{}();
      }

      return error;
    } else {
      return {};
    }
  }

 private:
  Descriptor raw_descriptor_{GetNull{}()};
};

}  // namespace pposix
