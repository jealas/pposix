#include "pposix/rt/mqueue.hpp"

#include "pposix/errno.hpp"

namespace pposix {

std::error_code mq_close_policy::operator()(mq_d mq_descriptor) const noexcept {
  if (const int res{::mq_close(underlying_value(mq_descriptor.raw()))}; res == -1) {
    return current_errno_code();
  } else {
    return {};
  }
}

result<unique_descriptor<mq_d, mq_close_policy>> mq_open(const char* name, mq_mode mode,
                                                         mq_option option) noexcept {
  pposix::mqd_t mq_descriptor{::mq_open(name, underlying_value(mode) | underlying_value(option))};
  if (mq_descriptor == pposix::mqd_t::null) {
    return current_errno_code();
  } else {
    return unique_descriptor<mq_d, mq_close_policy>{mq_d{mq_descriptor}};
  }
}

result<unique_descriptor<mq_d, mq_close_policy>> mq_open(const std::string& name, mq_mode mode,
                                                         mq_option option) noexcept {
  return pposix::mq_open(name.c_str(), mode, option);
}

result<mq_attr> mq_getattr(pposix::mq_d mq_descriptor) noexcept {
  ::mq_attr attributes{};

  const int result{::mq_getattr(underlying_value(mq_descriptor.raw()), &attributes)};
  if (result == -1) {
    return current_errno_code();
  } else {
    return mq_attr{attributes};
  }
}

}