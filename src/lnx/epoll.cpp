#include "pposix/lnx/epoll.hpp"

#include <unistd.h>

#include "pposix/errno.hpp"
#include "pposix/util.hpp"

namespace pposix::lnx {

namespace capi {

epoll_event::epoll_event(epoll_event_flag event_flags) noexcept : ::epoll_event{} {
  this->events = underlying_v(event_flags);
}

// NOTE: This constructor must call the epoll_event{epoll_event_flag} constructor to correctly
// initialize this class.
epoll_event::epoll_event(epoll_event_flag event_flags, void *data) noexcept
    : epoll_event{event_flags} {
  this->data.ptr = data;
}

epoll_event::epoll_event(epoll_event_flag event_flags, raw_fd data) noexcept
    : epoll_event{event_flags} {
  this->data.fd = data;
}

epoll_event::epoll_event(epoll_event_flag event_flags, uint32_t data) noexcept
    : epoll_event{event_flags} {
  this->data.u32 = data;
}

epoll_event::epoll_event(epoll_event_flag event_flags, uint64_t data) noexcept
    : epoll_event{event_flags} {
  this->data.u64 = data;
}

}  // namespace capi

// Epoll
epoll::epoll(raw_fd epoll_fd) noexcept : epoll_fd_{epoll_fd} {}

result<epoll> epoll::unsafe_create(size_t size) noexcept {
    PPOSIX_COMMON_RESULT_MAP_IMPL(epoll, ::epoll_create, size)}

result<epoll> epoll::unsafe_create1(capi::epoll_flag flags) noexcept {
    PPOSIX_COMMON_RESULT_MAP_IMPL(epoll, ::epoll_create1, underlying_v(flags))}

result<epoll> epoll::create() noexcept {
  constexpr static size_t NON_ZERO_SIZE = 1u;
  return unsafe_create(NON_ZERO_SIZE);
}

result<epoll> epoll::create(decltype(epoll_cloexec)) noexcept {
  return unsafe_create1(epoll_cloexec);
}

std::error_code epoll::unsafe_ctl(capi::epoll_operation op, raw_fd fd,
                                  capi::epoll_event *event) noexcept {
  return PPOSIX_COMMON_CALL(::epoll_ctl, *epoll_fd_, underlying_v(op), fd, event);
}

std::error_code epoll::ctl(epoll_add add) noexcept {
  return unsafe_ctl(capi::epoll_operation::remove, add.fd, &add.event);
}

std::error_code epoll::ctl(epoll_remove remove) noexcept {
  return unsafe_ctl(capi::epoll_operation::remove, remove.fd, nullptr);
}

std::error_code epoll::ctl(epoll_modify mod) noexcept {
  return unsafe_ctl(capi::epoll_operation::remove, mod.fd(), mod.event_ptr());
}

result<int> epoll::wait(span<lnx::epoll_event> events, milliseconds timeout) noexcept {
  // TODO: Assert that events.length() <= std::numeric_literals<int>::max()

  return PPOSIX_COMMON_CALL(::epoll_wait, *epoll_fd_, events.data(), events.length(),
                            timeout.count());
}

result<int> epoll::pwait(span<lnx::epoll_event> events, milliseconds timeout,
                         const sigset &sigmask) noexcept {
  // TODO: Assert that events.length() <= std::numeric_literals<int>::max()

  return PPOSIX_COMMON_CALL(::epoll_pwait, *epoll_fd_, events.data(), events.length(),
                            timeout.count(), sigmask.sigset_ptr());
}

}  // namespace pposix::lnx