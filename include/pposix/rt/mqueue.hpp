#pragma once

#include <system_error>

#include <fcntl.h>
#include <limits.h>
#include <mqueue.h>

#include "pposix/byte_span.hpp"
#include "pposix/descriptor.hpp"
#include "pposix/result.hpp"
#include "pposix/signal.hpp"
#include "pposix/stat.hpp"
#include "pposix/time.hpp"
#include "pposix/unique_descriptor.hpp"
#include "pposix/util.hpp"

namespace pposix::rt {

namespace detail {

struct mq_null_descriptor {
  ::mqd_t operator()() const noexcept { return ::mqd_t{-1}; }
};

}  // namespace detail

struct mq_tag {};

using mq_d = descriptor<mq_tag, ::mqd_t, detail::mq_null_descriptor>;

struct mq_close_policy {
  std::error_code operator()(mq_d mq_descriptor) const noexcept;
};

class mq_current_attr;

result<rt::mq_current_attr> mq_getattr(rt::mq_d mq_descriptor) noexcept;

using unique_mq_d = unique_descriptor<mq_d, mq_close_policy>;

namespace capi {

enum class mq_mode : int { read_only = O_RDONLY, write_only = O_WRONLY, read_write = O_RDWR };
enum class mq_option : int { create = O_CREAT, excl = O_EXCL, non_blocking = O_NONBLOCK };

result<unique_mq_d> mq_open(const char* name, mq_mode mode, mq_option option) noexcept;

}  // namespace capi

class mq_current_attr {
 public:
  constexpr mq_current_attr(::mq_attr attributes) noexcept : attributes_{attributes} {}

  constexpr long current_message_count() const noexcept { return attributes_.mq_curmsgs; }
  constexpr long max_message_count() const noexcept { return attributes_.mq_maxmsg; }
  constexpr long max_message_size() const noexcept { return attributes_.mq_msgsize; }

 private:
  ::mq_attr attributes_{};
};

// Message queue get attributes
result<rt::mq_current_attr> mq_getattr(mq_d mq_descriptor) noexcept;

// Message queue open
template <capi::mq_mode Flag>
using mq_mode_flag = exclusive_enum_flag<capi::mq_mode, Flag>;

inline constexpr mq_mode_flag<capi::mq_mode::read_only> read_only{};
inline constexpr mq_mode_flag<capi::mq_mode::write_only> write_only{};
inline constexpr mq_mode_flag<capi::mq_mode::read_write> read_write{};

template <capi::mq_option Flag>
using mq_option_flag = enum_flag<capi::mq_option, Flag>;

template <capi::mq_option Flags>
using mq_option_flag_set = enum_flag_set<capi::mq_option, Flags>;

inline constexpr mq_option_flag<capi::mq_option::excl> mq_excl{};
inline constexpr mq_option_flag<capi::mq_option::non_blocking> mq_non_blocking{};

template <capi::mq_mode Mode>
result<unique_mq_d> mq_open(const char* name, mq_mode_flag<Mode>) noexcept {
  const mq_d res{::mq_open(name, underlying_value(Mode))};
  if (res == null_descriptor) {
    return current_errno_code();
  } else {
    return unique_mq_d{mq_d{res}};
  }
}

template <capi::mq_mode Mode, capi::mq_option Flag>
result<unique_mq_d> mq_open(const char* name, mq_mode_flag<Mode>, mq_option_flag<Flag>) noexcept {
  static_assert(Flag != capi::mq_option::excl,
                "The mq_excl flag can only be used with the mq_open(name, mode, flags, "
                "mq_create_queue) overload.");

  const mq_d res{::mq_open(name, underlying_value(Mode) | underlying_value(Flag))};
  if (res == null_descriptor) {
    return current_errno_code();
  } else {
    return unique_mq_d{mq_d{res}};
  }
}

class mq_create_queue;

template <capi::mq_mode Mode>
result<unique_mq_d> mq_open(const char* name, mq_mode_flag<Mode>,
                            rt::mq_create_queue attributes) noexcept;

template <capi::mq_mode Mode, capi::mq_option Flags>
result<unique_mq_d> mq_open(const char* name, mq_mode_flag<Mode>, mq_option_flag_set<Flags>,
                            rt::mq_create_queue create_queue) noexcept;

class mq_create_queue {
 public:
  constexpr mq_create_queue(long max_message_count, long max_message_size) noexcept {
    attributes_.mq_maxmsg = max_message_count;
    attributes_.mq_msgsize = max_message_size;
  }

  template <capi::mq_mode Mode>
  friend result<unique_mq_d> mq_open(const char* name, mq_mode_flag<Mode>,
                                     rt::mq_create_queue attributes) noexcept;

  template <capi::mq_mode Mode, capi::mq_option Flags>
  friend result<unique_mq_d> mq_open(const char* name, mq_mode_flag<Mode>,
                                     mq_option_flag_set<Flags>,
                                     rt::mq_create_queue create_queue) noexcept;

 private:
  ::mq_attr* mq_attr_ptr() noexcept { return &attributes_; }

  ::mq_attr attributes_{};
};

template <capi::mq_mode Mode>
result<unique_mq_d> mq_open(const char* name, mq_mode_flag<Mode>,
                            rt::mq_create_queue attributes) noexcept {
  const mq_d res{::mq_open(name, underlying_value(Mode), attributes.mq_attr_ptr())};
  if (res == null_descriptor) {
    return current_errno_code();
  } else {
    return unique_mq_d{mq_d{res}};
  }
}

template <capi::mq_mode Mode, capi::mq_option Flags>
result<unique_mq_d> mq_open(const char* name, mq_mode_flag<Mode>, mq_option_flag_set<Flags>,
                            rt::mq_create_queue create_queue) noexcept {
  const mq_d res{::mq_open(name, underlying_value(Mode) | underlying_value(Flags),
                           create_queue.mq_attr_ptr())};
  if (res == null_descriptor) {
    return current_errno_code();
  } else {
    return unique_mq_d{mq_d{res}};
  }
}

// Message queue set attributes
template <capi::mq_option Option>
result<mq_current_attr> mq_setattr(mq_d d, mq_option_flag<Option>) {
  static_assert(not mq_option_flag_set<Option>::has(mq_excl),
                "mq_excl cannot be used with mq_setattr, since it's only relevant with mq_open.");

  ::mq_attr new_attributes{};
  new_attributes.mq_flags = underlying_value(Option);

  ::mq_attr current_attributes{};

  const int res{::mq_setattr(d.raw(), &new_attributes, &current_attributes)};
  if (res == -1) {
    return current_errno_code();
  } else {
    return mq_current_attr{current_attributes};
  }
}

enum class mq_message_priority : unsigned { max = _POSIX_MQ_PRIO_MAX };

class mq_message {
 public:
  constexpr mq_message(mq_message_priority priority, byte_span message) noexcept
      : priority_{priority}, message_{message} {}

  constexpr mq_message_priority priority() const noexcept { return priority_; }
  constexpr byte_span message_bytes() const noexcept { return message_; }

 private:
  mq_message_priority priority_{};
  byte_span message_{};
};

// Message queue receive
result<mq_message> mq_receive(mq_d mq, byte_span message_buffer) noexcept;

namespace capi {

[[nodiscard]] std::error_code mq_send(mq_d mq, byte_cspan message,
                                      mq_message_priority priority) noexcept;

}  // namespace capi

template <mq_message_priority Priority>
struct mq_static_message_priority {};

template <mq_message_priority Priority>
constexpr bool operator==(mq_static_message_priority<Priority>, mq_message_priority rhs) noexcept {
  return Priority == rhs;
}

template <mq_message_priority Priority>
constexpr bool operator==(mq_message_priority lhs, mq_static_message_priority<Priority>) noexcept {
  return Priority == lhs;
}

template <unsigned Priority>
inline constexpr mq_static_message_priority<mq_message_priority{Priority}>
    mq_make_static_message_priority{};

template <mq_message_priority Priority>
[[nodiscard]] std::error_code mq_send(mq_d mq, byte_cspan message,
                                      mq_static_message_priority<Priority>) noexcept {
  static_assert(Priority < mq_message_priority::max,
                "The message queue send priority cannot be greater than or equal to "
                "mq_message_priority::max (aka _POSIX_MQ_PRIO_MAX)");

  return capi::mq_send(mq, message, Priority);
}

std::error_code mq_unlink(const char* name) noexcept;
std::error_code mq_unlink(const std::string& name) noexcept;

struct {
} mq_deregister_notification;

std::error_code mq_notify(mq_d mq, decltype(mq_deregister_notification)) noexcept;

namespace capi {

std::error_code mq_notify(mq_d mq, const pposix::sigevent&) noexcept;

}

struct {
} mq_notify_none;

std::error_code mq_notify(mq_d mq, decltype(mq_notify_none)) noexcept;

class mq_notify_signal {
 public:
  inline mq_notify_signal(sig_number number, sig_event_notify_handler handler, int value) noexcept
      : event_{sig_notify::signal, number, handler, value} {}

  inline mq_notify_signal(sig_number number, sig_event_notify_handler handler,
                          void* value) noexcept
      : event_{sig_notify::signal, number, handler, value} {}

  const pposix::sigevent& event() const noexcept { return event_; }

 private:
  pposix::sigevent event_{};
};

std::error_code mq_notify(mq_d mq, mq_notify_signal notify_signal) noexcept;

// TODO: Implement thread-notify mq_notify when POSIX threads extension is implemented.

result<mq_message> mq_timed_receive(mq_d mq, byte_span data,
                                    const pposix::timespec& absolute_time) noexcept;

namespace capi {

[[nodiscard]] std::error_code mq_timed_send(mq_d mq, byte_cspan message,
                                            mq_message_priority priority,
                                            const pposix::timespec& absolute_time) noexcept;

}

template <mq_message_priority Priority>
[[nodiscard]] std::error_code mq_timed_send(mq_d mq, byte_cspan message,
                                            mq_static_message_priority<Priority>,
                                            const pposix::timespec& absolute_time) noexcept {
  static_assert(Priority < mq_message_priority::max,
                "The message queue send priority cannot be greater than or equal to "
                "mq_message_priority::max (aka _POSIX_MQ_PRIO_MAX)");

  return capi::mq_timed_send(mq, message, Priority, absolute_time);
}

}  // namespace pposix::rt
