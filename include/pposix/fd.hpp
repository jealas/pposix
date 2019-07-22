#include "pposix/null_fd.hpp"

namespace pposix {

using fd_t = int;
static constexpr fd_t NULLFD = -1;

template <class Tag>
class fd {
 public:
  fd() = default;
  constexpr explicit fd(fd_t fd) noexcept : fd_{fd} {}
  constexpr explicit fd(null_fd_t) noexcept : fd_{NULLFD} {}

  fd(const fd &) noexcept = default;
  fd(fd &&) noexcept = default;

  fd &operator=(const fd &) = default;
  fd &operator=(fd &&) = default;

  constexpr fd_t raw() const noexcept { return fd_; }

 private:
  fd_t fd_;
};

template <class Tag>
constexpr bool operator==(fd<Tag> lhs, fd<Tag> rhs) {
  return lhs.raw() == rhs.raw();
}

template <class Tag>
constexpr bool operator!=(fd<Tag> lhs, fd<Tag> rhs) {
  return lhs.raw() != rhs.raw();
}

template <class Tag>
constexpr bool operator==(fd<Tag> fd, null_fd_t) noexcept {
  return fd.raw() == NULLFD;
}

template <class Tag>
constexpr bool operator==(null_fd_t, fd<Tag> fd) noexcept {
  return fd == nullfd;
}

template <class Tag>
constexpr bool operator!=(fd<Tag> fd, null_fd_t) noexcept {
  return !(fd == nullfd);
}

template <class Tag>
constexpr bool operator!=(null_fd_t, fd<Tag> fd) noexcept {
  return fd != nullfd;
}

}