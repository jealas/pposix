#pragma once

#include <system_error>
#include <utility>

#include "pposix/errno.hpp"
#include "pposix/fd.hpp"
#include "pposix/null_fd.hpp"
#include "pposix/unique_fd.hpp"

namespace pposix {

struct fd_close_policy {
  std::error_code operator()(raw_fd fd) const noexcept;
};

template <class Fd, class ClosePolicy = fd_close_policy>
class [[nodiscard]] unique_fd : private ClosePolicy {
  static_assert(is_file_descriptor_v<Fd>);

 public:
  constexpr unique_fd() noexcept : unique_fd::unique_fd{nullfd} {}

  constexpr unique_fd(nullfd_t) noexcept(noexcept(ClosePolicy{}))
      : ClosePolicy{} {}  // NOLINT implicit constructor

  constexpr explicit unique_fd(Fd file_descriptor) noexcept(noexcept(ClosePolicy{}))
      : ClosePolicy{}, raw_fd_{file_descriptor} {}

  constexpr explicit unique_fd(Fd file_descriptor, const ClosePolicy &close) noexcept(
      noexcept(ClosePolicy{std::declval<const ClosePolicy &>()}))
      : raw_fd_{file_descriptor}, ClosePolicy{close} {}

  constexpr explicit unique_fd(Fd file_descriptor, ClosePolicy && close) noexcept(
      noexcept(ClosePolicy{std::declval<ClosePolicy &&>()}))
      : ClosePolicy{std::move(close)}, raw_fd_{file_descriptor} {}

  ~unique_fd() {
    if (const auto error = close()) {
      // TODO: Log this fatal error.
    }
  }

  unique_fd(const unique_fd &other) = delete;
  unique_fd(unique_fd && other) noexcept { std::swap(raw_fd_, other.raw_fd_); }

  unique_fd &operator=(const unique_fd &) = delete;
  unique_fd &operator=(unique_fd &&) = delete;

  bool empty() const noexcept { return raw_fd_ == nullfd; }
  explicit operator bool() const noexcept { return not empty(); }

  Fd raw() const noexcept { return raw_fd_; }
  Fd operator*() const noexcept { return raw(); }

  constexpr ClosePolicy &get_close_policy() noexcept { return *this; }
  constexpr const ClosePolicy &get_close_policy() const noexcept { return *this; }

  [[nodiscard]] Fd release() noexcept {
    Fd tmp_fd{raw_fd_};
    raw_fd_ = nullfd;
    return tmp_fd;
  }

  [[nodiscard]] std::error_code close() noexcept(
      noexcept(std::declval<ClosePolicy &>()(std::declval<Fd>()))) {
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
  Fd raw_fd_{nullfd};
};

}  // namespace pposix
