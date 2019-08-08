#pragma once

#include <system_error>
#include <utility>

#include "pposix/descriptor.hpp"
#include "pposix/errno.hpp"

namespace pposix {

template <class Descriptor, class ClosePolicy>
class [[nodiscard]] unique_descriptor : private ClosePolicy {
  static_assert(is_descriptor_v<Descriptor>);

 public:
  constexpr unique_descriptor() noexcept = default;

  constexpr unique_descriptor(null_descriptor_t) noexcept(noexcept(ClosePolicy{}))
      : ClosePolicy{} {}  // NOLINT implicit constructor

  constexpr explicit unique_descriptor(Descriptor descriptor) noexcept(noexcept(ClosePolicy{}))
      : ClosePolicy{}, raw_descriptor_{descriptor} {}

  constexpr explicit unique_descriptor(Descriptor descriptor, const ClosePolicy &close) noexcept(
      noexcept(ClosePolicy{std::declval<const ClosePolicy &>()}))
      : raw_descriptor_{descriptor}, ClosePolicy{close} {}

  constexpr explicit unique_descriptor(Descriptor descriptor, ClosePolicy && close) noexcept(
      noexcept(ClosePolicy{std::declval<ClosePolicy &&>()}))
      : ClosePolicy{std::move(close)}, raw_descriptor_{descriptor} {}

  ~unique_descriptor() {
    if (const auto error = close()) {
      // TODO: Log this fatal error.
    }
  }

  unique_descriptor(const unique_descriptor &other) = delete;
  unique_descriptor(unique_descriptor && other) noexcept {
    std::swap(raw_descriptor_, other.raw_descriptor_);
  }

  unique_descriptor &operator=(const unique_descriptor &) = delete;
  unique_descriptor &operator=(unique_descriptor &&) = delete;

  bool empty() const noexcept { return raw_descriptor_ == null_descriptor; }
  explicit operator bool() const noexcept { return not empty(); }

  Descriptor raw() const noexcept { return raw_descriptor_; }
  Descriptor operator*() const noexcept { return raw(); }

  constexpr ClosePolicy &get_close_policy() noexcept { return *this; }
  constexpr const ClosePolicy &get_close_policy() const noexcept { return *this; }

  [[nodiscard]] Descriptor release() noexcept {
    Descriptor tmp_fd{raw_descriptor_};
    raw_descriptor_ = null_descriptor;
    return tmp_fd;
  }

  [[nodiscard]] std::error_code close() noexcept(
      noexcept(std::declval<ClosePolicy &>()(std::declval<Descriptor>()))) {
    if (empty()) {
      return {};
    }

    const auto error = ClosePolicy::operator()(raw());
    if (not error) {
      raw_descriptor_ = null_descriptor;
    }

    return error;
  }

 private:
  Descriptor raw_descriptor_{null_descriptor};
};

}  // namespace pposix
