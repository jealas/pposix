#pragma once

#include "pposix/capi/nullfd.hpp"

namespace pposix::capi {

using fd_t = int;

namespace detail {

static constexpr fd_t NULLFD = -1;

}

class rawfd {
 public:
  rawfd() = default;
  constexpr explicit rawfd(fd_t fd) noexcept : fd_{fd} {}
  constexpr explicit rawfd(nullfd_t) noexcept : fd_{detail::NULLFD} {}

  rawfd(const rawfd &) = default;
  rawfd(rawfd &&) = default;

  rawfd &operator=(const rawfd &) = default;
  rawfd &operator=(rawfd &&) = default;

  constexpr rawfd &operator=(nullfd_t) noexcept {
    fd_ = detail::NULLFD;
    return *this;
  }

  constexpr fd_t fd() const noexcept { return fd_; }

 private:
  int fd_{detail::NULLFD};
};

constexpr bool operator==(rawfd lhs, rawfd rhs) { return lhs.fd() == rhs.fd(); }
constexpr bool operator!=(rawfd lhs, rawfd rhs) { return lhs.fd() != rhs.fd(); }

constexpr bool operator==(rawfd fd, nullfd_t) noexcept { return fd.fd() == detail::NULLFD; }
constexpr bool operator==(nullfd_t, rawfd fd) noexcept { return fd == nullfd; }

constexpr bool operator!=(rawfd fd, nullfd_t) noexcept { return !(fd == nullfd); }
constexpr bool operator!=(nullfd_t, rawfd fd) noexcept { return fd != nullfd; }

}  // namespace pposix::capi
