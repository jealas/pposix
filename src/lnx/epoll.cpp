#include "pposix/lnx/epoll.hpp"

#include <unistd.h>

#include "pposix/errno.hpp"
#include "pposix/util.hpp"

namespace pposix::lnx {

namespace capi {

result<unique_epoll_fd> epoll_create(size_t size) noexcept {
  if (const int res{::epoll_create(size)}; res < 0) {
    return current_errno_code();
  } else {
    return unique_epoll_fd{epoll_fd{res}};
  }
}

result<unique_epoll_fd> epoll_create1(epoll_flag flags) noexcept {
  if (const int res{::epoll_create1(underlying_value(flags))}; res < 0) {
    return current_errno_code();
  } else {
    return unique_epoll_fd{epoll_fd{res}};
  }
}

epoll_event::epoll_event(epoll_event_flag event_flags) noexcept : ::epoll_event{} {
  this->events = underlying_value(event_flags);
}

// NOTE: This constructor must call the epoll_event{epoll_event_flag} constructor to correctly
// initialize this class.
epoll_event::epoll_event(epoll_event_flag event_flags, void *data) noexcept
    : epoll_event{event_flags} {
  this->data.ptr = data;
}

epoll_event::epoll_event(epoll_event_flag event_flags, raw_fd data) noexcept
    : epoll_event{event_flags} {
  this->data.fd = data.raw();
}

epoll_event::epoll_event(epoll_event_flag event_flags, uint32_t data) noexcept
    : epoll_event{event_flags} {
  this->data.u32 = data;
}

epoll_event::epoll_event(epoll_event_flag event_flags, uint64_t data) noexcept
    : epoll_event{event_flags} {
  this->data.u64 = data;
}

std::error_code epoll_ctl(epoll_fd epoll_fdescriptor, epoll_operation op, raw_fd fd,
                          epoll_event event) noexcept {
  const int res{::epoll_ctl(epoll_fdescriptor.raw(), underlying_value(op), fd.raw(), &event)};
  if (res == -1) {
    return current_errno_code();
  } else {
    return {};
  }
}

result<unsigned> epoll_wait(epoll_fd epoll_fdescriptor, span<capi::epoll_event> events,
                            milliseconds timeout) noexcept {
  // TODO: Assert that events.length() <= std::numeric_literals<int>::max()

  const int res{
      ::epoll_wait(epoll_fdescriptor.raw(), events.data(), events.length(), timeout.count())};
  if (res == -1) {
    return current_errno_code();
  } else {
    static_assert(sizeof(int) <= sizeof(unsigned));
    // TODO: Assert that res >= 0.
    return static_cast<unsigned>(res);
  }
}

result<unsigned> epoll_pwait(epoll_fd epoll_fdescriptor, span<capi::epoll_event> events,
                             milliseconds timeout, const sigset &sigmask) noexcept {
  // TODO: Assert that events.length() <= std::numeric_literals<int>::max()

  const int res{::epoll_pwait(epoll_fdescriptor.raw(), events.data(), events.length(),
                              timeout.count(), sigmask.sigset_ptr())};
  if (res == -1) {
    return current_errno_code();
  } else {
    static_assert(sizeof(int) <= sizeof(unsigned));
    // TODO: Assert that res >= 0.
    return static_cast<unsigned>(res);
  }
}

}  // namespace capi

result<unique_epoll_fd> epoll_create() noexcept {
  constexpr static size_t NON_ZERO_SIZE = 1u;
  return capi::epoll_create(NON_ZERO_SIZE);
}

result<unique_epoll_fd> epoll_create(decltype(epoll_cloexec)) noexcept {
  return capi::epoll_create1(epoll_cloexec);
}

}  // namespace pposix::lnx
