#include "pposix/rt/mqueue.hpp"

#include "pposix/errno.hpp"

namespace pposix::rt {

std::error_code close_mq_d(const ::mqd_t mq_descriptor) noexcept {
  return PPOSIX_COMMON_CALL(::mq_close, mq_descriptor);
}

mq::mq(::mqd_t mq_d) noexcept : mq_d_{mq_d} {}

result<mq> mq::unsafe_open(const char* name, capi::mq_mode mode, capi::mq_option option) noexcept {
  const mqd_t mq_descriptor{::mq_open(name, underlying_v(mode) | underlying_v(option))};
  if (mq_descriptor == NULL_MQD_T) {
    return current_errno_code();
  } else {
    return mq{mq_descriptor};
  }
}

result<mq_current_attr> mq::getattr() noexcept {
  ::mq_attr attributes{};

  const int result{::mq_getattr(*mq_d_, &attributes)};
  if (result == -1) {
    return current_errno_code();
  } else {
    return mq_current_attr{attributes};
  }
}

result<mq_message> mq::receive(byte_span message_buffer) noexcept {
  unsigned priority{};

  const ssize_t num_message_bytes{
      ::mq_receive(*mq_d_, static_cast<char*>(static_cast<void*>(message_buffer.data())),
                   message_buffer.length(), &priority)};
  if (num_message_bytes == -1) {
    return current_errno_code();
  } else {
    return mq_message{mq_message_priority{priority},
                      byte_span{message_buffer.data(), static_cast<size_t>(num_message_bytes)}};
  }
}

[[nodiscard]] std::error_code mq::unsafe_send(byte_cspan message,
                                              mq_message_priority priority) noexcept {
  return PPOSIX_COMMON_CALL(::mq_send, *mq_d_,
                            static_cast<const char*>(static_cast<const void*>(message.data())),
                            message.length(), underlying_v(priority));
}

std::error_code mq::unlink(const char* name) noexcept {
  return PPOSIX_COMMON_CALL(::mq_unlink, name);
}

std::error_code mq::notify(decltype(mq_deregister_notification)) noexcept {
  const ::sigevent* null_sigevent{nullptr};
  return PPOSIX_COMMON_CALL(::mq_notify, *mq_d_, null_sigevent);
}

std::error_code mq::unsafe_notify(const pposix::sigevent& sigevent) noexcept {
  return PPOSIX_COMMON_CALL(::mq_notify, *mq_d_, &sigevent);
}

std::error_code mq::notify(decltype(mq_notify_none)) noexcept {
  const pposix::sigevent event{sig_notify::none, sig_number{}, nullptr, nullptr};
  return unsafe_notify(event);
}

std::error_code mq::notify(mq_notify_signal notify_signal) noexcept {
  return unsafe_notify(notify_signal.event());
}

result<mq_message> mq::timed_receive(byte_span message,
                                     const pposix::timespec& absolute_time) noexcept {
  unsigned priority{};
  const auto res{::mq_timedreceive(*mq_d_, static_cast<char*>(static_cast<void*>(message.data())),
                                   message.length(), &priority, &absolute_time)};
  if (res == -1) {
    return current_errno_code();
  } else {
    return mq_message{mq_message_priority{priority},
                      byte_span{message.data(), static_cast<size_t>(res)}};
  }
}

[[nodiscard]] std::error_code mq::unsafe_timed_send(
    byte_cspan message, mq_message_priority priority,
    const pposix::timespec& absolute_time) noexcept {
  return PPOSIX_COMMON_CALL(::mq_timedsend, *mq_d_,
                            static_cast<const char*>(static_cast<const void*>(message.data())),
                            message.length(), underlying_v(priority), &absolute_time);
}

}  // namespace pposix::rt