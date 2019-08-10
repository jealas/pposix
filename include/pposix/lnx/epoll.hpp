#pragma once

#include <cstddef>

#include <sys/epoll.h>

#include "pposix/descriptor.hpp"
#include "pposix/duration.hpp"
#include "pposix/fd.hpp"
#include "pposix/result.hpp"
#include "pposix/signal.hpp"
#include "pposix/span.hpp"
#include "pposix/unique_descriptor.hpp"
#include "pposix/util.hpp"

namespace pposix::lnx {

enum class epoll_d_t : int {};
using epoll_d = descriptor<epoll_d_t, epoll_d_t{-1}>;

struct epoll_close_policy {
  std::error_code operator()(epoll_d descriptor) const noexcept;
};

using unique_epoll_d = unique_descriptor<epoll_d, epoll_close_policy>;

namespace capi {

enum class epoll_flag { cloexec = EPOLL_CLOEXEC };

result<unique_epoll_d> epoll_create(size_t) noexcept;
result<unique_epoll_d> epoll_create1(epoll_flag flags) noexcept;

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

// Forward declarations for friend functions
class epoll_event;

std::error_code epoll_ctl(epoll_d epoll_descriptor, epoll_operation op, raw_fd fd,
                          capi::epoll_event event) noexcept;
result<unsigned> epoll_wait(epoll_d epoll_descriptor, capi::epoll_event *event, int maxevents,
                            milliseconds timeout) noexcept;
result<unsigned> epoll_pwait(int epfd, span<capi::epoll_event>, milliseconds timeout,
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

std::error_code epoll_ctl(epoll_d epoll_descriptor, epoll_operation op, raw_fd fd,
                          capi::epoll_event event) noexcept;

result<unsigned> epoll_wait(epoll_d epoll_descriptor, span<capi::epoll_event> events,
                            milliseconds timeout) noexcept;

result<unsigned> epoll_pwait(epoll_d epoll_descriptor, span<capi::epoll_event> events,
                             milliseconds timeout, const sigset &sigmask) noexcept;

}  // namespace capi

}  // namespace pposix::lnx
