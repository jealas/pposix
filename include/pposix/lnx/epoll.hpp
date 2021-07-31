#pragma once

#include <sys/epoll.h>

#include <cstddef>

#include "pposix/duration.hpp"
#include "pposix/fd.hpp"
#include "pposix/result.hpp"
#include "pposix/signal.hpp"
#include "pposix/span.hpp"
#include "pposix/util.hpp"

namespace pposix::lnx {

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

static_assert(sizeof(capi::epoll_event) == sizeof(::epoll_event));
static_assert(alignof(capi::epoll_event) == alignof(::epoll_event));

static result<epoll_fd> create(int) noexcept;
static result<epoll_fd> create1(capi::epoll_flag flags) noexcept;

std::error_code ctl(raw_epoll_fd, capi::epoll_operation, raw_fd, capi::epoll_event *) noexcept;

}  // namespace capi

inline constexpr enum_flag<capi::epoll_flag, capi::epoll_flag::cloexec> epoll_cloexec{};

// Epoll flag types
template <capi::epoll_event_flag Flag>
using epoll_event_flag = enum_flag<capi::epoll_event_flag, Flag>;

inline constexpr epoll_event_flag<capi::epoll_event_flag::read_available> epoll_read_available{};
inline constexpr epoll_event_flag<capi::epoll_event_flag::write_available> epoll_write_available{};
inline constexpr epoll_event_flag<capi::epoll_event_flag::socket_closed> epoll_socket_closed{};
inline constexpr epoll_event_flag<capi::epoll_event_flag::fd_exception> epoll_fd_exception{};
inline constexpr epoll_event_flag<capi::epoll_event_flag::fd_error> epoll_fd_error{};
inline constexpr epoll_event_flag<capi::epoll_event_flag::fd_hup> epoll_fd_hup{};
inline constexpr epoll_event_flag<capi::epoll_event_flag::edge_triggered> epoll_edge_triggered{};
inline constexpr epoll_event_flag<capi::epoll_event_flag::one_shot> epoll_one_shot{};
inline constexpr epoll_event_flag<capi::epoll_event_flag::wakeup> epoll_wakeup{};
inline constexpr epoll_event_flag<capi::epoll_event_flag::exclusive> epoll_exclusive{};

class epoll;

// Epoll control commands
struct epoll_add {
  raw_fd fd;
  lnx::capi::epoll_event event;
};

struct epoll_remove {
  raw_fd fd;
};

class epoll_modify {
  template <capi::epoll_event_flag Flags>
  static constexpr void check_epoll_modify_flags() noexcept {
    static_assert(not epoll_event_flag<Flags>::has(epoll_exclusive),
                  "epoll_exclusive cannot be used with epoll_modify");
  }

 public:
  template <capi::epoll_event_flag Flags>
  constexpr epoll_modify(const raw_fd fd, epoll_event_flag<Flags>) noexcept
      : fd_{fd}, event_{Flags} {
    check_epoll_modify_flags<Flags>();
  }

  template <capi::epoll_event_flag Flags>
  constexpr epoll_modify(const raw_fd fd, epoll_event_flag<Flags>, void *const data) noexcept
      : fd_{fd}, event_{Flags, data} {
    check_epoll_modify_flags<Flags>();
  }

  template <capi::epoll_event_flag Flags>
  constexpr epoll_modify(const raw_fd fd, epoll_event_flag<Flags>, const raw_fd data) noexcept
      : fd_{fd}, event_{Flags, data} {
    check_epoll_modify_flags<Flags>();
  }

  template <capi::epoll_event_flag Flags>
  constexpr epoll_modify(raw_fd fd, epoll_event_flag<Flags>, uint32_t data) noexcept
      : fd_{fd}, event_{Flags, data} {
    check_epoll_modify_flags<Flags>();
  }

  template <capi::epoll_event_flag Flags>
  constexpr epoll_modify(raw_fd fd, epoll_event_flag<Flags>, uint64_t data) noexcept
      : fd_{fd}, event_{Flags, data} {
    check_epoll_modify_flags<Flags>();
  }

  friend class epoll;

 private:
  constexpr raw_fd fd() const noexcept { return fd_; }
  constexpr capi::epoll_event *event_ptr() noexcept { return &event_; }

  raw_fd fd_{-1};
  capi::epoll_event event_{};
};

class epoll_event final : public ::epoll_event {
 public:
  epoll_event() noexcept = default;

  inline bool read_available() const noexcept { return has_flag(lnx::epoll_read_available); }
  inline bool write_available() const noexcept { return has_flag(lnx::epoll_write_available); }
  inline bool socket_closed() const noexcept { return has_flag(lnx::epoll_socket_closed); }
  inline bool fd_exception() const noexcept { return has_flag(lnx::epoll_fd_exception); }
  inline bool fd_error() const noexcept { return has_flag(lnx::epoll_fd_error); }
  inline bool fd_hup() const noexcept { return has_flag(lnx::epoll_fd_hup); }

 private:
  template <capi::epoll_event_flag Flag>
  inline bool has_flag(epoll_event_flag<Flag>) const noexcept {
    return this->events & underlying_v(Flag);
  }
};

class epoll {
 public:
  epoll() noexcept = default;

  explicit epoll(capi::epoll_fd fd) noexcept;

  epoll(const epoll &) noexcept = delete;
  epoll(epoll &&) noexcept = default;

  epoll &operator=(const epoll &) noexcept = delete;
  epoll &operator=(epoll &&) noexcept = default;

  static result<epoll> create() noexcept;
  static result<epoll> create(decltype(epoll_cloexec)) noexcept;

  std::error_code ctl(epoll_add add) noexcept;

  std::error_code ctl(epoll_remove remove) noexcept;

  std::error_code ctl(epoll_modify mod) noexcept;

  result<int> wait(span<lnx::epoll_event>, milliseconds timeout) noexcept;

  result<int> pwait(span<lnx::epoll_event>, milliseconds timeout, const sigset &sigmask) noexcept;

 private:
  capi::epoll_fd epoll_fd_{};
};

}  // namespace pposix::lnx
