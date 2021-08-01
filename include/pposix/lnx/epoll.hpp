#pragma once

#include <sys/epoll.h>

#include <cstddef>

#include "pposix/duration.hpp"
#include "pposix/fd.hpp"
#include "pposix/result.hpp"
#include "pposix/signal.hpp"
#include "pposix/span.hpp"
#include "pposix/util.hpp"

namespace pposix {

namespace capi {

struct raw_epoll_fd : raw_fd {
  using raw_fd::raw_fd;
};

using epoll_fd_t = raw_fd_t;

using epoll_fd =
    descriptor<raw_epoll_fd, descriptor_constant<raw_epoll_fd, epoll_fd_t, -1>, close_raw_fd>;

enum class epoll_flag { cloexec = EPOLL_CLOEXEC };

enum class epoll_event_flag : uint32_t {
  read_available = EPOLLIN,
  write_available = EPOLLOUT,
  socket_closed = EPOLLRDHUP,
  fd_exception = EPOLLPRI,
  fd_error = EPOLLERR,
  fd_hup = EPOLLHUP,
  edge_triggered = EPOLLET,
  one_shot = EPOLLONESHOT,
  wakeup = EPOLLWAKEUP,
  exclusive = EPOLLEXCLUSIVE
};

enum class epoll_modify_flag : uint32_t {
  read_available = EPOLLIN,
  write_available = EPOLLOUT,
  socket_closed = EPOLLRDHUP,
  fd_exception = EPOLLPRI,
  fd_error = EPOLLERR,
  fd_hup = EPOLLHUP,
  edge_triggered = EPOLLET,
  one_shot = EPOLLONESHOT,
  wakeup = EPOLLWAKEUP,
};

enum class epoll_operation : int {
  add = EPOLL_CTL_ADD,
  remove = EPOLL_CTL_DEL,
  modify = EPOLL_CTL_MOD
};

// Forward declaration
class epoll_event;

class epoll_event final : public ::epoll_event {
 public:
  epoll_event() noexcept = default;

  explicit epoll_event(epoll_event_flag event_flags) noexcept;
  epoll_event(epoll_event_flag event_flags, void *data) noexcept;
  epoll_event(epoll_event_flag event_flags, raw_fd data) noexcept;
  epoll_event(epoll_event_flag event_flags, uint32_t data) noexcept;
  epoll_event(epoll_event_flag event_flags, uint64_t data) noexcept;
};

static_assert(sizeof(epoll_event) == sizeof(::epoll_event));
static_assert(alignof(epoll_event) == alignof(::epoll_event));

static result<epoll_fd> create(int) noexcept;
static result<epoll_fd> create1(epoll_flag flags) noexcept;

std::error_code ctl(raw_epoll_fd, epoll_operation, raw_fd, epoll_event *) noexcept;

}  // namespace capi

class epoll;

class epoll_event final : public ::epoll_event {
 public:
  epoll_event() noexcept = default;

  constexpr bool read_available() const noexcept {
    return has_flag(capi::epoll_event_flag::read_available);
  }
  constexpr bool write_available() const noexcept {
    return has_flag(capi::epoll_event_flag::write_available);
  }
  constexpr bool socket_closed() const noexcept {
    return has_flag(capi::epoll_event_flag::socket_closed);
  }
  constexpr bool fd_exception() const noexcept {
    return has_flag(capi::epoll_event_flag::fd_exception);
  }
  constexpr bool fd_error() const noexcept { return has_flag(capi::epoll_event_flag::fd_error); }
  constexpr bool fd_hup() const noexcept { return has_flag(capi::epoll_event_flag::fd_hup); }

 private:
  constexpr bool has_flag(const capi::epoll_event_flag flag) const noexcept {
    return this->events & underlying_v(flag);
  }
};

class epoll {
 public:
  ~epoll() = default;
  epoll() noexcept = default;

  explicit epoll(capi::epoll_fd fd) noexcept;

  epoll(const epoll &) noexcept = delete;
  epoll(epoll &&) noexcept = default;

  epoll &operator=(const epoll &) noexcept = delete;
  epoll &operator=(epoll &&) noexcept = default;

  static result<epoll> create() noexcept;
  static result<epoll> create_cloexec() noexcept;

  std::error_code add(raw_fd, capi::epoll_event) noexcept;
  std::error_code remove(raw_fd) noexcept;
  std::error_code modify(raw_fd, capi::epoll_modify_flag) noexcept;

  result<int> wait(span<epoll_event>, milliseconds timeout) noexcept;
  result<int> pwait(span<epoll_event>, milliseconds timeout, const sigset &sigmask) noexcept;

 private:
  capi::epoll_fd epoll_fd_{};
};

}  // namespace pposix
