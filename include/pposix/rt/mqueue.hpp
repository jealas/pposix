#pragma once

#include <system_error>

#include <mqueue.h>

#include "pposix/descriptor.hpp"
#include "pposix/result.hpp"
#include "pposix/unique_descriptor.hpp"
#include "pposix/util.hpp"

namespace pposix {

enum class mqd_t : ::mqd_t { null = -1 };

using mq_d = descriptor<pposix::mqd_t, pposix::mqd_t::null>;

enum class mq_mode : int { read_only = O_RDONLY, write_only = O_WRONLY, read_write = O_RDWR };

enum class mq_option : int { create = O_CREAT, excl = O_EXCL, non_blocking = O_NONBLOCK };

struct mq_close_policy {
  std::error_code operator()(mq_d mq_descriptor) const noexcept;
};

result<unique_descriptor<mq_d, mq_close_policy>> mq_open(const char* name, mq_mode mode,
                                                         mq_option option) noexcept;

result<unique_descriptor<mq_d, mq_close_policy>> mq_open(const std::string& name, mq_mode mode,
                                                         mq_option option) noexcept;

/*
The <mqueue.h> header shall define the mqd_t type, which is used for message queue descriptors.
This is not an array type.

The <mqueue.h> header shall define the pthread_attr_t, size_t, and ssize_t types as described in
<sys/types.h>.

The <mqueue.h> header shall define the struct timespec structure as described in <time.h>.

The tag sigevent shall be declared as naming an incomplete structure type, the contents of which
are described in the <signal.h> header.

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

Inclusion of the <mqueue.h> header may make visible symbols defined in the headers <fcntl.h>,
<signal.h>, and <time.h>.
 */

// Forward declarations
class mq_attr;
result<mq_attr> mq_getattr(pposix::mq_d mq_descriptor) noexcept;

class mq_attr {
  constexpr long current_queued_message_count() const noexcept { return attributes_.mq_curmsgs; }
  constexpr long max_message_count() const noexcept { return attributes_.mq_maxmsg; }
  constexpr long max_message_size() const noexcept { return attributes_.mq_msgsize; }

  constexpr bool is(mq_mode mode) const noexcept {
    return attributes_.mq_flags & underlying_value(mode);
  }

  constexpr bool is(mq_option option) const noexcept {
    return attributes_.mq_flags & underlying_value(option);
  }

  friend result<mq_attr> mq_getattr(pposix::mq_d mq_descriptor) noexcept;

 private:
  constexpr mq_attr(::mq_attr attributes) noexcept : attributes_{attributes} {}

  ::mq_attr attributes_{};
};

result<mq_attr> mq_getattr(pposix::mq_d mq_descriptor) noexcept;

}  // namespace pposix
