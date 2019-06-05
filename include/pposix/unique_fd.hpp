#ifndef PPOSIX_UNIQUE_FD_HPP
#define PPOSIX_UNIQUE_FD_HPP

#include <type_traits>
#include <utility>

#include "pposix/default_close_policy.hpp"
#include "pposix/errno_code.hpp"
#include "pposix/nullfd.hpp"
#include "pposix/rawfd.hpp"

namespace pposix {

template <class Tag, class ClosePolicy = default_close_policy>
class [[nodiscard]] unique_fd {
 public:
  constexpr unique_fd() noexcept : unique_fd::unique_fd{nullfd} {}

  constexpr /*implicit*/ unique_fd(nullfd_t) noexcept : close_{} {}

  constexpr explicit unique_fd(const rawfd file_descriptor) noexcept(noexcept(ClosePolicy{}))
      : raw_fd_{file_descriptor}, close_{} {}

  constexpr explicit unique_fd(const rawfd file_descriptor, const ClosePolicy &close) noexcept(
      noexcept(ClosePolicy{std::declval<const ClosePolicy &>()}))
      : raw_fd_{file_descriptor}, close_{close} {}

  constexpr explicit unique_fd(const rawfd file_descriptor, ClosePolicy &&close) noexcept(
      noexcept(ClosePolicy{std::declval<ClosePolicy &&>()}))
      : raw_fd_{file_descriptor}, close_{std::move(close)} {}

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

  rawfd raw() const noexcept { return raw_fd_; }

  ClosePolicy &get_close_policy() noexcept { return close_; }

  const ClosePolicy &get_close_policy() const noexcept { return close_; }

  [[nodiscard]] rawfd release() noexcept {
    const auto tmp_fd = raw_fd_;
    raw_fd_ = nullfd;
    return tmp_fd;
  }

  [[nodiscard]] std::error_code close() noexcept(
      noexcept(std::declval<ClosePolicy &>()(std::declval<rawfd>()))) {
    if (not empty()) {
      while (const auto ec = close_(raw())) {
        if (ec == std::errc::interrupted) {
          continue;
        } else {
          return ec;
        }
      }

      raw_fd_ = nullfd;
    }

    return {};
  }

 private:
  rawfd raw_fd_{nullfd};
  ClosePolicy close_;
};

}  // namespace pposix

#endif  // PPOSIX_UNIQUE_FD_HPP
