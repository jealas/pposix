#pragma once

#include <system_error>
#include <utility>

#include "pposix/errno.hpp"

namespace pposix {

struct null_d_t {};
inline constexpr null_d_t null_d{};

template <class FdWrapper, class UnderlyingFd, UnderlyingFd Fd>
struct descriptor_constant {
  constexpr FdWrapper operator()() const noexcept
  {
    return FdWrapper{Fd};
  }
};

template <class Descriptor, class GetNull, auto ClosePolicy>
class [[nodiscard]] descriptor {
  static_assert(noexcept(GetNull{}()));
  static_assert(noexcept(Descriptor{GetNull{}()}));
  static_assert(noexcept(Descriptor{std::declval<Descriptor>()}));
  static_assert(noexcept(ClosePolicy(std::declval<Descriptor>())));

 public:
  constexpr descriptor() noexcept = default;

  constexpr explicit descriptor(null_d_t) noexcept {}

  constexpr explicit descriptor(Descriptor descriptor) : raw_descriptor_{descriptor} {}

  ~descriptor() {
    if (const auto error = close()) {
      // TODO: Log this fatal error.
    }
  }

  descriptor(const descriptor &other) = delete;
  descriptor(descriptor &&other) noexcept {
    std::swap(raw_descriptor_, other.raw_descriptor_);
  }

  descriptor &operator=(const descriptor &) = delete;

  constexpr descriptor &operator=(descriptor &&other) noexcept {
    std::swap(raw_descriptor_, other.raw_descriptor_);
  }

  [[nodiscard]] constexpr bool empty() const noexcept { return raw_descriptor_ == GetNull{}(); }
  constexpr bool operator==(null_d_t) const noexcept { return empty(); }

  constexpr explicit operator bool() const noexcept { return not empty(); }

  constexpr Descriptor raw() const noexcept { return raw_descriptor_; }
  constexpr Descriptor operator*() const noexcept { return raw(); }

  constexpr Descriptor *operator->() noexcept { return &raw_descriptor_; }
  constexpr Descriptor const *operator->() const noexcept { return &raw_descriptor_; }

  [[nodiscard]] Descriptor release() noexcept {
    Descriptor tmp_fd{raw_descriptor_};
    raw_descriptor_ = GetNull{}();
    return tmp_fd;
  }

  [[nodiscard]] std::error_code close() noexcept {
    const auto error = ClosePolicy(raw());
    if (not error) {
      raw_descriptor_ = GetNull{}();
    }

    return error;
  }

 private:
  Descriptor raw_descriptor_{GetNull{}()};
};

}  // namespace pposix
