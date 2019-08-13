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

namespace capi {

result<unique_descriptor<mq_d, mq_close_policy>> mq_open(const char* name, mq_mode mode,
                                                         mq_option option) noexcept {
  mq_d mq_descriptor{::mq_open(name, underlying_value(mode) | underlying_value(option))};
  if (mq_descriptor == null_descriptor) {
    return current_errno_code();
  } else {
    return unique_descriptor<mq_d, mq_close_policy>{mq_d{mq_descriptor}};
  }
}

}  // namespace capi

result<rt::mq_attr> mq_getattr(rt::mq_d mq_descriptor) noexcept {
  ::mq_attr attributes{};

  const int result{::mq_getattr(mq_descriptor.raw(), &attributes)};
  if (result == -1) {
    return current_errno_code();
  } else {
    return rt::mq_attr{attributes};
  }
}

}