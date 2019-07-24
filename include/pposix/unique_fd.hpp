#pragma once

#include <system_error>
#include <utility>

#include "pposix/default_close_policy.hpp"
#include "pposix/errno.hpp"
#include "pposix/fd.hpp"
#include "pposix/null_fd.hpp"

namespace pposix {

template <class Fd, class ClosePolicy = default_close_policy>
class [[nodiscard]] unique_fd {
  static_assert(is_file_descriptor_v<Fd>);

 public:
  constexpr unique_fd() noexcept : unique_fd::unique_fd{nullfd} {}

  constexpr unique_fd(null_fd_t) noexcept : close_{} {}  // NOLINT implicit constructor

  constexpr explicit unique_fd(const Fd file_descriptor) noexcept(noexcept(ClosePolicy{}))
      : raw_fd_{file_descriptor}, close_{} {}

  constexpr explicit unique_fd(const Fd file_descriptor, const ClosePolicy &close) noexcept(
      noexcept(ClosePolicy{std::declval<const ClosePolicy &>()}))
      : raw_fd_{file_descriptor}, close_{close} {}

  constexpr explicit unique_fd(const Fd file_descriptor, ClosePolicy &&close) noexcept(
      noexcept(ClosePolicy{std::declval<ClosePolicy &&>()}))
      : raw_fd_{file_descriptor}, close_{std::move(close)} {}

  ~unique_fd() noexcept(false) {
    if (const auto error = close()) {
      throw std::system_error{error,
                              "Failed to automatically close file descriptor. Try manually "
                              "calling close() to catch and handle the error"};
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

  constexpr ClosePolicy &get_close_policy() noexcept { return close_; }
  constexpr const ClosePolicy &get_close_policy() const noexcept { return close_; }

  [[nodiscard]] Fd release() noexcept {
    const auto tmp_fd = raw_fd_;
    raw_fd_ = Fd{nullfd};
    return tmp_fd;
  }

  [[nodiscard]] std::error_code close() noexcept(
      noexcept(std::declval<ClosePolicy &>()(std::declval<Fd>()))) {
    if (empty()) {
      return {};
    }

    while (const auto ec = close_(raw())) {
      if (ec == std::errc::interrupted) {
        continue;
      } else {
        return ec;
      }
    }

    raw_fd_ = Fd{nullfd};

    return {};
  }

 private:
  Fd raw_fd_{nullfd};
  ClosePolicy close_{};
};

}  // namespace pposix
