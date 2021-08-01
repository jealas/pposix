#pragma once

#include <fcntl.h>
#include <limits.h>
#include <mqueue.h>

#include <system_error>

#include "pposix/byte_span.hpp"
#include "pposix/descriptor.hpp"
#include "pposix/result.hpp"
#include "pposix/signal.hpp"
#include "pposix/stat.hpp"
#include "pposix/time.hpp"
#include "pposix/util.hpp"

namespace pposix::rt {

constexpr ::mqd_t NULL_MQD_T{-1};

namespace detail {

struct mq_null_descriptor {
  ::mqd_t operator()() const noexcept { return NULL_MQD_T; }
};

}  // namespace detail

namespace capi {

enum class mq_mode : int { read_only = O_RDONLY, write_only = O_WRONLY, read_write = O_RDWR };
enum class mq_option : int { create = O_CREAT, excl = O_EXCL, non_blocking = O_NONBLOCK };

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

class mq;

class mq_create_queue {
 public:
  constexpr mq_create_queue(long max_message_count, long max_message_size) noexcept {
    attributes_.mq_maxmsg = max_message_count;
    attributes_.mq_msgsize = max_message_size;
  }

  friend class mq;

 private:
  ::mq_attr* mq_attr_ptr() noexcept { return &attributes_; }

  ::mq_attr attributes_{};
};

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

struct {
} mq_deregister_notification;

struct {
} mq_notify_none;

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

std::error_code close_mq_d(::mqd_t) noexcept;

using unique_mq_d = descriptor<::mqd_t, detail::mq_null_descriptor, close_mq_d>;

class mq {
 public:
  mq() noexcept = default;

  explicit mq(::mqd_t mq_d) noexcept;

  mq(const mq&) = delete;
  mq(mq&&) noexcept = default;

  mq& operator=(const mq&) = delete;
  mq& operator=(mq&&) noexcept = default;

  static result<mq> unsafe_open(const char* name, capi::mq_mode mode,
                                capi::mq_option option) noexcept;

  static inline result<mq> open(const char* name, capi::mq_mode mode) noexcept {
    const mqd_t res{::mq_open(name, underlying_v(mode))};
    if (res == NULL_MQD_T) {
      return current_errno_code();
    } else {
      return mq{res};
    }
  }

  template <capi::mq_mode Mode, capi::mq_option Flag>
  inline result<mq> open(const char* name, capi::mq_mode mode, capi::mq_option options) noexcept {
    static_assert(Flag != capi::mq_option::excl,
                  "The mq_excl flag can only be used with the mq_open(name, mode, flags, "
                  "mq_create_queue) overload.");

    const ::mqd_t res{::mq_open(name, underlying_v(mode) | underlying_v(options))};
    if (res == NULL_MQD_T) {
      return current_errno_code();
    } else {
      return mq{res};
    }
  }

  template <capi::mq_mode Mode>
  inline result<mq> open(const char* name, capi::mq_mode mode,
                         mq_create_queue attributes) noexcept {
    const ::mqd_t res{::mq_open(name, underlying_v(mode), attributes.mq_attr_ptr())};
    if (res == NULL_MQD_T) {
      return current_errno_code();
    } else {
      return mq{res};
    }
  }

  template <capi::mq_mode Mode, capi::mq_option Flags>
  inline result<mq> open(const char* name, capi::mq_mode mode, capi::mq_option options,
                         mq_create_queue create_queue) noexcept {
    const ::mqd_t res{
        ::mq_open(name, underlying_v(mode) | underlying_v(options), create_queue.mq_attr_ptr())};
    if (res == NULL_MQD_T) {
      return current_errno_code();
    } else {
      return mq{res};
    }
  }

  static std::error_code unlink(const char* name) noexcept;

  result<mq_current_attr> getattr() noexcept;

  result<mq_current_attr> setattr(capi::mq_option option) {
    ::mq_attr new_attributes{};
    new_attributes.mq_flags = underlying_v(option);

    ::mq_attr current_attributes{};

    const int res{::mq_setattr(mq_d_.raw(), &new_attributes, &current_attributes)};
    if (res == -1) {
      return current_errno_code();
    } else {
      return mq_current_attr{current_attributes};
    }
  }

  [[nodiscard]] std::error_code unsafe_send(byte_cspan message,
                                            mq_message_priority priority) noexcept;

  template <mq_message_priority Priority>
  [[nodiscard]] std::error_code send(byte_cspan message,
                                     mq_static_message_priority<Priority>) noexcept {
    static_assert(Priority < mq_message_priority::max,
                  "The message queue send priority cannot be greater than or equal to "
                  "mq_message_priority::max (aka _POSIX_MQ_PRIO_MAX)");

    return unsafe_send(message, Priority);
  }

  result<mq_message> receive(byte_span message_buffer) noexcept;

  std::error_code notify(decltype(mq_deregister_notification)) noexcept;

  std::error_code unsafe_notify(const pposix::sigevent&) noexcept;

  std::error_code notify(decltype(mq_notify_none)) noexcept;

  std::error_code notify(mq_notify_signal notify_signal) noexcept;

  // TODO: Implement thread-notify mq_notify when POSIX threads extension is implemented.

  result<mq_message> timed_receive(byte_span data, const pposix::timespec& absolute_time) noexcept;

  [[nodiscard]] std::error_code unsafe_timed_send(byte_cspan message, mq_message_priority priority,
                                                  const pposix::timespec& absolute_time) noexcept;

  template <mq_message_priority Priority>
  [[nodiscard]] std::error_code timed_send(byte_cspan message,
                                           mq_static_message_priority<Priority>,
                                           const pposix::timespec& absolute_time) noexcept {
    static_assert(Priority < mq_message_priority::max,
                  "The message queue send priority cannot be greater than or equal to "
                  "mq_message_priority::max (aka _POSIX_MQ_PRIO_MAX)");

    return unsafe_timed_send(message, Priority, absolute_time);
  }

 private:
  unique_mq_d mq_d_{};
};

}  // namespace pposix::rt
