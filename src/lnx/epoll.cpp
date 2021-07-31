#include "pposix/lnx/epoll.hpp"

#include <unistd.h>

#include "pposix/errno.hpp"
#include "pposix/util.hpp"

namespace pposix::lnx {

namespace capi {

result<epoll_fd> create(const int size) noexcept {
    PPOSIX_COMMON_RESULT_DOUBLE_WRAP_IMPL(epoll_fd, raw_epoll_fd, ::epoll_create, size)}

result<epoll_fd> create1(const capi::epoll_flag flags) noexcept {
    PPOSIX_COMMON_RESULT_DOUBLE_WRAP_IMPL(epoll_fd, raw_epoll_fd, ::epoll_create1,
                                          underlying_v(flags))}

std::error_code ctl(raw_epoll_fd epoll_fd, capi::epoll_operation op, raw_fd fd,
                    capi::epoll_event *event) noexcept {
  return PPOSIX_COMMON_CALL(::epoll_ctl, static_cast<raw_fd_t>(epoll_fd), underlying_v(op),
                            static_cast<raw_fd_t>(fd), event);
}

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
  this->data.fd = static_cast<raw_fd_t>(data);
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
epoll::epoll(capi::epoll_fd fd) noexcept : epoll_fd_{std::move(fd)} {}

result<epoll> epoll::create() noexcept {
  constexpr static size_t NON_ZERO_SIZE = 1u;
  return result_map<epoll>(capi::create(NON_ZERO_SIZE),
                           [](capi::epoll_fd fd) noexcept { return epoll{std::move(fd)}; });
}

result<epoll> epoll::create(decltype(epoll_cloexec)) noexcept {
  return result_map<epoll>(capi::create1(epoll_cloexec),
                           [](capi::epoll_fd fd) noexcept { return epoll{std::move(fd)}; });
}

std::error_code epoll::ctl(epoll_add add) noexcept {
  return capi::ctl(*epoll_fd_, capi::epoll_operation::add, add.fd, &add.event);
}

std::error_code epoll::ctl(epoll_remove remove) noexcept {
  return capi::ctl(*epoll_fd_, capi::epoll_operation::remove, remove.fd, nullptr);
}

std::error_code epoll::ctl(epoll_modify mod) noexcept {
  return capi::ctl(*epoll_fd_, capi::epoll_operation::modify, mod.fd(), mod.event_ptr());
}

result<int> epoll::wait(span<lnx::epoll_event> events, milliseconds timeout) noexcept {
  // TODO: Assert that events.length() <= std::numeric_literals<int>::max()

  return PPOSIX_COMMON_CALL(::epoll_wait, static_cast<raw_fd_t>(*epoll_fd_), events.data(),
                            events.length(), timeout.count());
}

result<int> epoll::pwait(span<lnx::epoll_event> events, milliseconds timeout,
                         const sigset &sigmask) noexcept {
  // TODO: Assert that events.length() <= std::numeric_literals<int>::max()

  return PPOSIX_COMMON_CALL(::epoll_pwait, static_cast<raw_fd_t>(*epoll_fd_), events.data(),
                            events.length(), timeout.count(), sigmask.sigset_ptr());
}

}  // namespace pposix::lnx
