#pragma once

#include <cstddef>

#include <sys/epoll.h>

#include "pposix/duration.hpp"
#include "pposix/fd.hpp"
#include "pposix/result.hpp"
#include "pposix/signal.hpp"
#include "pposix/span.hpp"
#include "pposix/unique_fd.hpp"
#include "pposix/util.hpp"

namespace pposix::lnx {

struct epoll_fd_tag {};
using epoll_fd = fd<epoll_fd_tag>;

using unique_epoll_fd = unique_fd<epoll_fd>;

namespace capi {

enum class epoll_flag { cloexec = EPOLL_CLOEXEC };

result<unique_epoll_fd> epoll_create(size_t) noexcept;
result<unique_epoll_fd> epoll_create1(epoll_flag flags) noexcept;

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
  change_settings = EPOLL_CTL_MOD
};

// Forward declaration
class epoll_event;

std::error_code epoll_ctl(epoll_fd epoll_fd, epoll_operation op, raw_fd fd,
                          capi::epoll_event event) noexcept;
result<unsigned> epoll_wait(epoll_fd epoll_fd, capi::epoll_event *event, int maxevents,
                            milliseconds timeout) noexcept;
result<unsigned> epoll_pwait(epoll_fd epoll_fd, span<capi::epoll_event>, milliseconds timeout,
                             const sigset &sigmask) noexcept;

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

constexpr epoll_event_flag operator|(epoll_event_flag lhs, epoll_event_flag rhs) noexcept {
  return epoll_event_flag{underlying_value(lhs) | underlying_value(rhs)};
}

}  // namespace capi

result<unique_epoll_fd> epoll_create() noexcept;

inline constexpr enum_constant<capi::epoll_flag, capi::epoll_flag::cloexec> epoll_cloexec{};

result<unique_epoll_fd> epoll_create(decltype(epoll_cloexec)) noexcept;

}  // namespace pposix::lnx
