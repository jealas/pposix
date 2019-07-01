#pragma once

#include "pposix/capi/null_fd.hpp"

namespace pposix::capi {

using fd_t = int;

namespace detail {

static constexpr fd_t NULLFD = -1;

}

class raw_fd {
 public:
  raw_fd() = default;
  constexpr explicit raw_fd(fd_t fd) noexcept : fd_{fd} {}
  constexpr explicit raw_fd(null_fd_t) noexcept : fd_{detail::NULLFD} {}

  raw_fd(const raw_fd &) = default;
  raw_fd(raw_fd &&) = default;

  raw_fd &operator=(const raw_fd &) = default;
  raw_fd &operator=(raw_fd &&) = default;

  constexpr fd_t fd() const noexcept { return fd_; }

 private:
  int fd_{detail::NULLFD};
};

constexpr bool operator==(raw_fd lhs, raw_fd rhs) { return lhs.fd() == rhs.fd(); }
constexpr bool operator!=(raw_fd lhs, raw_fd rhs) { return lhs.fd() != rhs.fd(); }

constexpr bool operator==(raw_fd fd, null_fd_t) noexcept { return fd.fd() == detail::NULLFD; }
constexpr bool operator==(null_fd_t, raw_fd fd) noexcept { return fd == nullfd; }

constexpr bool operator!=(raw_fd fd, null_fd_t) noexcept { return !(fd == nullfd); }
constexpr bool operator!=(null_fd_t, raw_fd fd) noexcept { return fd != nullfd; }

}  // namespace pposix::capi
