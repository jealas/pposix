#pragma once

#include <system_error>
#include <utility>

#include "pposix/errno.hpp"

namespace pposix {

struct null_d_t {};
inline constexpr null_d_t null_d{};

template <class Tag, class Descriptor, class GetNull, class ClosePolicy>
class [[nodiscard]] unique_d : private ClosePolicy {
 public:
  constexpr unique_d() noexcept = default;

  constexpr unique_d(null_d_t) noexcept(noexcept(ClosePolicy{}))
      : ClosePolicy{} {}  // NOLINT implicit constructor

  constexpr explicit unique_d(Descriptor descriptor) noexcept(noexcept(ClosePolicy{}))
      : ClosePolicy{}, raw_descriptor_{descriptor} {}

  constexpr explicit unique_d(Descriptor descriptor, const ClosePolicy &close) noexcept(
      noexcept(ClosePolicy{std::declval<const ClosePolicy &>()}))
      : raw_descriptor_{descriptor}, ClosePolicy{close} {}

  constexpr explicit unique_d(Descriptor descriptor, ClosePolicy && close) noexcept(
      noexcept(ClosePolicy{std::declval<ClosePolicy &&>()}))
      : ClosePolicy{std::move(close)}, raw_descriptor_{descriptor} {}

  ~unique_d() {
    if (const auto error = close()) {
      // TODO: Log this fatal error.
    }
  }

  unique_d(const unique_d &other) = delete;
  unique_d(unique_d && other) noexcept { std::swap(raw_descriptor_, other.raw_descriptor_); }

  unique_d &operator=(const unique_d &) = delete;

  unique_d &operator=(unique_d &&other) noexcept {
    std::swap(raw_descriptor_, other.raw_descriptor_);
  }

  bool empty() const noexcept { return raw_descriptor_ == GetNull{}(); }
  bool operator==(null_d_t) const noexcept { return empty(); }

  explicit operator bool() const noexcept { return not empty(); }

  Descriptor raw() const noexcept { return raw_descriptor_; }
  Descriptor operator*() const noexcept { return raw(); }

  Descriptor *operator->() noexcept { return &raw_descriptor_; }
  Descriptor const *operator->() const noexcept { return &raw_descriptor_; }

  constexpr ClosePolicy &get_close_policy() noexcept { return *this; }
  constexpr const ClosePolicy &get_close_policy() const noexcept { return *this; }

  [[nodiscard]] Descriptor release() noexcept {
    Descriptor tmp_fd{raw_descriptor_};
    raw_descriptor_ = GetNull{}();
    return tmp_fd;
  }

  [[nodiscard]] std::error_code close() noexcept(
      noexcept(std::declval<ClosePolicy &>()(std::declval<Descriptor>()))) {
    if (empty()) {
      return {};
    }

    const auto error = ClosePolicy::operator()(raw());
    if (not error) {
      raw_descriptor_ = GetNull{}();
    }

    return error;
  }

 private:
  Descriptor raw_descriptor_{GetNull{}()};
};

}  // namespace pposix
