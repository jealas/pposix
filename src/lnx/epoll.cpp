#include "pposix/lnx/epoll.hpp"

#include <unistd.h>

#include "pposix/errno.hpp"
#include "pposix/util.hpp"

namespace pposix::lnx {

std::error_code epoll_close_policy::operator()(epoll_d descriptor) const noexcept {
  return ::close(underlying_value(descriptor.raw())) == -1 ? current_errno_code()
                                                           : std::error_code{};
}

namespace capi {

result<unique_epoll_d> epoll_create(size_t size) noexcept {
  if (const epoll_d_t res{::epoll_create(size)}; res < epoll_d_t{0}) {
    return current_errno_code();
  } else {
    return unique_epoll_d{epoll_d{res}};
  }
}

result<unique_epoll_d> epoll_create1(epoll_flag flags) noexcept {
  if (const epoll_d_t res{::epoll_create1(underlying_value(flags))}; res < epoll_d_t{0}) {
    return current_errno_code();
  } else {
    return unique_epoll_d{epoll_d{res}};
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

std::error_code epoll_ctl(epoll_d epoll_descriptor, epoll_operation op, raw_fd fd,
                          epoll_event event) noexcept {
  const int res{::epoll_ctl(underlying_value(epoll_descriptor.raw()), underlying_value(op),
                            fd.raw(), &event)};
  if (res == -1) {
    return current_errno_code();
  } else {
    return {};
  }
}

result<unsigned> epoll_wait(epoll_d epoll_descriptor, span<capi::epoll_event> events,
                            milliseconds timeout) noexcept {
  // TODO: Assert that events.length() <= std::numeric_literals<int>::max()

  const int res{::epoll_wait(underlying_value(epoll_descriptor.raw()), events.data(),
                             events.length(), timeout.count())};
  if (res == -1) {
    return current_errno_code();
  } else {
    static_assert(sizeof(int) <= sizeof(unsigned));
    // TODO: Assert that res >= 0.
    return static_cast<unsigned>(res);
  }
}

result<unsigned> epoll_pwait(epoll_d epoll_descriptor, span<capi::epoll_event> events,
                             milliseconds timeout, const sigset &sigmask) noexcept {
  // TODO: Assert that events.length() <= std::numeric_literals<int>::max()

  const int res{::epoll_pwait(underlying_value(epoll_descriptor.raw()), events.data(),
                              events.length(), timeout.count(), sigmask.sigset_ptr())};
  if (res == -1) {
    return current_errno_code();
  } else {
    static_assert(sizeof(int) <= sizeof(unsigned));
    // TODO: Assert that res >= 0.
    return static_cast<unsigned>(res);
  }
}

}  // namespace capi

}  // namespace pposix::lnx
