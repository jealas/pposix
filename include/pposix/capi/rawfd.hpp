#pragma once

namespace pposix::capi {

using fd_t = int;

class rawfd {
 public:
  rawfd() = default;
  constexpr explicit rawfd(fd_t fd) noexcept : fd_{fd} {}

  rawfd(const rawfd &) = default;
  rawfd(rawfd &&) = default;

  rawfd &operator=(const rawfd &) = default;
  rawfd &operator=(rawfd &&) = default;

  constexpr fd_t fd() const noexcept { return fd_; }

 private:
  int fd_{-1};
};

bool operator==(rawfd lhs, rawfd rhs) { return lhs.fd() == rhs.fd(); }
bool operator!=(rawfd lhs, rawfd rhs) { return lhs.fd() != rhs.fd(); }

}  // namespace pposix::capi
