#pragma once

#include <system_error>
#include <utility>

#include "pposix/errno.hpp"
#include "pposix/fd.hpp"
#include "pposix/null_fd.hpp"
#include "pposix/unique_fd.hpp"

namespace pposix {

template <class Descriptor, class ClosePolicy>
class [[nodiscard]] unique_descriptor : private ClosePolicy {
  static_assert(is_file_descriptor_v<Descriptor>);

 public:
  constexpr unique_descriptor() noexcept : unique_descriptor::unique_descriptor{nullfd} {}

  constexpr unique_descriptor(nullfd_t) noexcept(noexcept(ClosePolicy{}))
      : ClosePolicy{} {}  // NOLINT implicit constructor

  constexpr explicit unique_descriptor(Descriptor file_descriptor) noexcept(noexcept(ClosePolicy{}))
      : ClosePolicy{}, raw_fd_{file_descriptor} {}

  constexpr explicit unique_descriptor(Descriptor file_descriptor, const ClosePolicy &close) noexcept(
      noexcept(ClosePolicy{std::declval<const ClosePolicy &>()}))
      : raw_fd_{file_descriptor}, ClosePolicy{close} {}

  constexpr explicit unique_descriptor(Descriptor file_descriptor, ClosePolicy && close) noexcept(
      noexcept(ClosePolicy{std::declval<ClosePolicy &&>()}))
      : ClosePolicy{std::move(close)}, raw_fd_{file_descriptor} {}

  ~unique_descriptor() {
    if (const auto error = close()) {
      // TODO: Log this fatal error.
    }
  }

  unique_descriptor(const unique_descriptor &other) = delete;
  unique_descriptor(unique_descriptor && other) noexcept { std::swap(raw_fd_, other.raw_fd_); }

  unique_descriptor &operator=(const unique_descriptor &) = delete;
  unique_descriptor &operator=(unique_descriptor &&) = delete;

  bool empty() const noexcept { return raw_fd_ == nullfd; }
  explicit operator bool() const noexcept { return not empty(); }

  Descriptor raw() const noexcept { return raw_fd_; }
  Descriptor operator*() const noexcept { return raw(); }

  constexpr ClosePolicy &get_close_policy() noexcept { return *this; }
  constexpr const ClosePolicy &get_close_policy() const noexcept { return *this; }

  [[nodiscard]] Descriptor release() noexcept {
    Descriptor tmp_fd{raw_fd_};
    raw_fd_ = nullfd;
    return tmp_fd;
  }

  [[nodiscard]] std::error_code close() noexcept(
      noexcept(std::declval<ClosePolicy &>()(std::declval<Descriptor>()))) {
    if (empty()) {
      return {};
    }

    const auto error = ClosePolicy::operator()(raw());
    if (not error) {
      raw_fd_ = nullfd;
    }

    return error;
  }

 private:
  Descriptor raw_fd_{nullfd};
};

}  // namespace pposix
