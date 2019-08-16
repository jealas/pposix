#include "pposix/rt/mqueue.hpp"

#include "pposix/errno.hpp"

namespace pposix::rt {

std::error_code mq_close_policy::operator()(mq_d mq_descriptor) const noexcept {
  if (const int res{::mq_close(mq_descriptor.raw())}; res == -1) {
    return current_errno_code();
  } else {
    return {};
  }
}

using unique_mq_descriptor = unique_descriptor<mq_d, mq_close_policy>;

namespace capi {

result<unique_mq_descriptor> mq_open(const char* name, mq_mode mode, mq_option option) noexcept {
  mq_d mq_descriptor{::mq_open(name, underlying_value(mode) | underlying_value(option))};
  if (mq_descriptor == null_descriptor) {
    return current_errno_code();
  } else {
    return unique_mq_descriptor{mq_d{mq_descriptor}};
  }
}

}  // namespace capi

result<rt::mq_current_attr> mq_getattr(rt::mq_d mq_descriptor) noexcept {
  ::mq_attr attributes{};

  const int result{::mq_getattr(mq_descriptor.raw(), &attributes)};
  if (result == -1) {
    return current_errno_code();
  } else {
    return rt::mq_current_attr{attributes};
  }
}

result<mq_message> mq_receive(mq_d mq, byte_span message_buffer) noexcept {
  unsigned priority{};

  const ssize_t num_message_bytes{
      ::mq_receive(mq.raw(), static_cast<char*>(static_cast<void*>(message_buffer.data())),
                   message_buffer.length(), &priority)};
  if (num_message_bytes == -1) {
    return current_errno_code();
  } else {
    return mq_message{mq_message_priority{priority},
                      byte_span{message_buffer.data(), static_cast<size_t>(num_message_bytes)}};
  }
}

namespace capi {

[[nodiscard]] std::error_code mq_send(mq_d mq, byte_cspan message,
                                      mq_message_priority priority) noexcept {
  const int res{::mq_send(mq.raw(),
                          static_cast<const char*>(static_cast<const void*>(message.data())),
                          message.length(), underlying_value(priority))};
  if (res == -1) {
    return current_errno_code();
  } else {
    return {};
  }
}

}  // namespace capi

std::error_code mq_unlink(const char* name) noexcept {
  return ::mq_unlink(name) == -1 ? current_errno_code() : std::error_code{};
}

std::error_code mq_unlink(const std::string& name) noexcept { return rt::mq_unlink(name.c_str()); }

std::error_code mq_notify(mq_d mq, decltype(mq_deregister_notification)) noexcept {
  const ::sigevent* null_sigevent{nullptr};
  return ::mq_notify(mq.raw(), null_sigevent) == -1 ? current_errno_code() : std::error_code{};
}

namespace capi {

std::error_code mq_notify(mq_d mq, const pposix::sigevent& sigevent) noexcept {
  return ::mq_notify(mq.raw(), &sigevent) == -1 ? current_errno_code() : std::error_code{};
}

}  // namespace capi

std::error_code mq_notify(mq_d mq, decltype(mq_notify_none)) noexcept {
  const pposix::sigevent event{sig_notify::none, sig_number{}, nullptr, nullptr};
  return capi::mq_notify(mq, event);
}

std::error_code mq_notify(mq_d mq, mq_notify_signal notify_signal) noexcept {
  return capi::mq_notify(mq, notify_signal.event());
}

result<mq_message> mq_timed_receive(mq_d mq, byte_span message,
                                    const pposix::timespec& absolute_time) noexcept {
  unsigned priority{};
  const auto res{::mq_timedreceive(mq.raw(),
                                   static_cast<char*>(static_cast<void*>(message.data())),
                                   message.length(), &priority, &absolute_time)};
  if (res == -1) {
    return current_errno_code();
  } else {
    return mq_message{mq_message_priority{priority},
                      byte_span{message.data(), static_cast<size_t>(res)}};
  }
}

namespace capi {

[[nodiscard]] std::error_code mq_timed_send(mq_d mq, byte_cspan message,
                                            mq_message_priority priority,
                                            const pposix::timespec& absolute_time) noexcept {
  return ::mq_timedsend(mq.raw(),
                        static_cast<const char*>(static_cast<const void*>(message.data())),
                        message.length(), underlying_value(priority), &absolute_time) == -1
             ? current_errno_code()
             : std::error_code{};
}

}  // namespace capi

}