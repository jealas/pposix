#pragma once

#include <system_error>

#include <mqueue.h>

#include "pposix/descriptor.hpp"
#include "pposix/result.hpp"
#include "pposix/unique_descriptor.hpp"
#include "pposix/util.hpp"

namespace pposix::rt {

enum class mqd_t : ::mqd_t { null = -1 };
using mq_d = descriptor<rt::mqd_t, rt::mqd_t::null>;

struct mq_close_policy {
  std::error_code operator()(mq_d mq_descriptor) const noexcept;
};

class mq_attr;

result<rt::mq_attr> mq_getattr(rt::mq_d mq_descriptor) noexcept;

namespace capi {

enum class mq_mode : int { read_only = O_RDONLY, write_only = O_WRONLY, read_write = O_RDWR };
enum class mq_option : int { create = O_CREAT, excl = O_EXCL, non_blocking = O_NONBLOCK };

result<unique_descriptor<mq_d, mq_close_policy>> mq_open(const char* name, mq_mode mode,
                                                         mq_option option) noexcept;

}  // namespace capi

/*
The following shall be declared as functions and may also be defined as macros. Function prototypes
shall be provided.

int      mq_notify(mqd_t, const struct sigevent *);
mqd_t    mq_open(const char *, int, ...);
ssize_t  mq_receive(mqd_t, char *, size_t, unsigned *);
int      mq_send(mqd_t, const char *, size_t, unsigned);
int      mq_setattr(mqd_t, const struct mq_attr *restrict,
             struct mq_attr *restrict);
ssize_t  mq_timedreceive(mqd_t, char *restrict, size_t,
             unsigned *restrict, const struct timespec *restrict);
int      mq_timedsend(mqd_t, const char *, size_t, unsigned,
             const struct timespec *);
int      mq_unlink(const char *);
 */

class mq_attr {
  constexpr long current_message_count() const noexcept { return attributes_.mq_curmsgs; }
  constexpr long max_message_count() const noexcept { return attributes_.mq_maxmsg; }
  constexpr long max_message_size() const noexcept { return attributes_.mq_msgsize; }

  friend result<rt::mq_attr> rt::mq_getattr(rt::mq_d mq_descriptor) noexcept;

 private:
  constexpr mq_attr(::mq_attr attributes) noexcept : attributes_{attributes} {}

  ::mq_attr attributes_{};
};

result<rt::mq_attr> mq_getattr(rt::mq_d mq_descriptor) noexcept;

}  // namespace pposix::rt
