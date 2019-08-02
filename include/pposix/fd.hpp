#pragma once

#include <type_traits>

#include "pposix/null_fd.hpp"

namespace pposix {

using fd_t = int;
static constexpr fd_t NULLFD = -1;

template <class>
class fd;

struct raw_fd_tag {};

using raw_fd = fd<raw_fd_tag>;

template <class Tag>
class fd {
 public:
  fd() = default;
  constexpr explicit fd(fd_t file_descriptor) noexcept : fd_{file_descriptor} {}
  constexpr explicit fd(nullfd_t) noexcept : fd_{NULLFD} {}

  fd(const fd &) noexcept = delete;
  fd(fd &&other) noexcept { std::swap(fd_, other.fd_); }

  fd &operator=(const fd &) noexcept = delete;
  fd &operator=(fd &&other) noexcept {
    std::swap(fd_, other.fd_);
    return *this;
  };

  fd &operator=(nullfd_t) noexcept {
    fd_ = NULLFD;
    return *this;
  }

  constexpr fd_t raw() const noexcept { return fd_; }

  constexpr operator raw_fd() const noexcept { return raw_fd{raw()}; }

 private:
  fd_t fd_{NULLFD};
};

template <class Tag>
constexpr bool operator==(const fd<Tag> &lhs, const fd<Tag> &rhs) {
  return lhs.raw() == rhs.raw();
}

template <class Tag>
constexpr bool operator!=(const fd<Tag> &lhs, const fd<Tag> &rhs) {
  return lhs.raw() != rhs.raw();
}

template <class Tag>
constexpr bool operator==(const fd<Tag> &fd, nullfd_t) noexcept {
  return fd.raw() == NULLFD;
}

template <class Tag>
constexpr bool operator==(nullfd_t, const fd<Tag> &fd) noexcept {
  return fd == nullfd;
}

template <class Tag>
constexpr bool operator!=(const fd<Tag> &fd, nullfd_t) noexcept {
  return not(fd == nullfd);
}

template <class Tag>
constexpr bool operator!=(nullfd_t, const fd<Tag> &fd) noexcept {
  return fd != nullfd;
}

template <class>
struct is_file_descriptor : std::false_type {};

template <class Tag>
struct is_file_descriptor<fd<Tag>> : std::true_type {};

template <class Fd>
constexpr bool is_file_descriptor_v = is_file_descriptor<Fd>::value;

}