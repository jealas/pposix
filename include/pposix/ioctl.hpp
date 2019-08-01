#pragma once

#include <memory>
#include <string_view>

#include <stropts.h>

#include "pposix/any_span.hpp"
#include "pposix/duration.hpp"
#include "pposix/fd.hpp"
#include "pposix/result.hpp"
#include "pposix/types.hpp"
#include "pposix/util.hpp"

namespace pposix {

// File ioctl
using ioctl_result = int;

enum class ioctl_request : int {};

result<ioctl_result> ioctl(raw_fd, ioctl_request, int) noexcept;
result<ioctl_result> ioctl(raw_fd, ioctl_request, void *) noexcept;
result<ioctl_result> ioctl(raw_fd, ioctl_request, const void *) noexcept;
result<ioctl_result> ioctl(raw_fd, ioctl_request, nullptr_t) noexcept;
result<ioctl_result> ioctl(raw_fd, ioctl_request, any_span) noexcept;
result<ioctl_result> ioctl(raw_fd, ioctl_request, any_cspan) noexcept;

struct ioctl_push {
  char *const name;
};

struct {
} ioctl_pop;

struct ioctl_look {
  char *name;
};

enum ioctl_flush : int { read_queues = FLUSHR, write_queues = FLUSHW, all_queues = FLUSHRW };

result<ioctl_result> ioctl(raw_fd, ioctl_push) noexcept;
result<ioctl_result> ioctl(raw_fd, decltype(ioctl_pop)) noexcept;
result<ioctl_result> ioctl(raw_fd, ioctl_look) noexcept;
result<ioctl_result> ioctl(raw_fd, ioctl_flush) noexcept;

struct ioctl_flush_band {
  ioctl_flush command;
  unsigned char priority;

  constexpr operator ::bandinfo() const noexcept {
    ::bandinfo info{};
    info.bi_flag = underlying_value(command);
    info.bi_pri = priority;

    return info;
  }
};

result<ioctl_result> ioctl(raw_fd, ioctl_flush_band) noexcept;

enum class ioctl_signal : int {
  none = 0,
  read_normal = S_RDNORM,
  read_band = S_RDBAND,
  input = S_INPUT,
  high_priority = S_HIPRI,
  output = S_OUTPUT,
  write_normal = S_WRNORM,
  write_band = S_WRBAND,
  message = S_MSG,
  error = S_ERROR,
  hangup = S_HANGUP,
  bandurg = S_BANDURG
};

constexpr ioctl_signal operator|(ioctl_signal lhs, ioctl_signal rhs) {
  return ioctl_signal{underlying_value(lhs) | underlying_value(rhs)};
}

constexpr ioctl_signal &operator|=(ioctl_signal &lhs, ioctl_signal rhs) {
  lhs = (lhs | rhs);
  return lhs;
}

struct ioctl_set_signal {
  ioctl_signal signals{};
};

result<ioctl_result> ioctl(raw_fd, ioctl_set_signal) noexcept;

struct {
} ioctl_get_signal;

result<ioctl_signal> ioctl(raw_fd, decltype(ioctl_get_signal)) noexcept;

struct ioctl_find {
  const char *name;
};

result<bool> ioctl(raw_fd, ioctl_find) noexcept;

struct {
} ioctl_peek;

// TODO: Wrap strpeek

result<strpeek> ioctl(raw_fd, decltype(ioctl_peek)) noexcept;

enum class ioctl_read_mode : int {
  byte_stream = RNORM,
  message_discard = RMSGD,
  message_nodiscard = RMSGN
};

enum class ioctl_control_option : int {
  normal = RPROTNORM,
  deliver = RPROTDAT,
  discard = RPROTDIS
};

class ioctl_read_option {
 public:
  constexpr ioctl_read_option(ioctl_read_mode read_mode, ioctl_control_option control_option)
      : raw_read_option_{underlying_value(read_mode) | underlying_value(control_option)} {}

  constexpr explicit ioctl_read_option(int raw_read_option) : raw_read_option_{raw_read_option} {}

  constexpr bool is(ioctl_read_mode read_mode) const noexcept {
    return raw_read_option_ & underlying_value(read_mode);
  }

  constexpr bool is(ioctl_control_option control_option) const noexcept {
    return raw_read_option_ & underlying_value(control_option);
  }

  constexpr int raw_read_option() const noexcept { return raw_read_option_; }

 private:
  int raw_read_option_;
};

constexpr ioctl_read_option operator|(ioctl_read_mode m, ioctl_control_option o) noexcept {
  return ioctl_read_option{m, o};
}

struct ioctl_set_read_option {
  ioctl_read_option read_option;
};

result<ioctl_result> ioctl(raw_fd, ioctl_set_read_option) noexcept;

struct {
} ioctl_get_read_option;

result<ioctl_read_option> ioctl(raw_fd, decltype(ioctl_get_read_option)) noexcept;

class next_message_count {
 public:
  constexpr next_message_count(int next_message_length, int message_count) noexcept
      : next_message_length_{}, message_count_{message_count} {}

  bool is_zero_length_message() const noexcept {
    return message_count_ != 0 and next_message_length_ == 0;
  }

  int next_message_length() const noexcept { return next_message_length_; }
  int message_count() const noexcept { return message_count_; }

 private:
  int next_message_length_{};
  int message_count_{};
};

struct {
} ioctl_get_next_message_count{};

result<next_message_count> ioctl(raw_fd, decltype(ioctl_get_next_message_count)) noexcept;

// TODO: Implement FDINSERT

static constexpr seconds ioctl_infinite_timeout{-1};
static constexpr seconds ioctl_implementation_defined_timeout{0};

class ioctl_command {
 public:
  constexpr ioctl_command(ioctl_request request, seconds timeout, any_span data) noexcept {
    command_.ic_cmd = underlying_value(request);
    command_.ic_timout = timeout.count();
    command_.ic_dp = static_cast<char *>(data.data());
    command_.ic_len = data.length();
  }

  constexpr ioctl_request request() const noexcept { return ioctl_request{command_.ic_cmd}; }

  constexpr seconds timeout() const noexcept { return seconds{command_.ic_timout}; }

  constexpr any_span data() const noexcept {
    return any_span{command_.ic_dp, static_cast<size_t>(command_.ic_len)};
  }

  constexpr explicit operator ::strioctl() const noexcept { return command_; }

 private:
  ::strioctl command_{};
};

struct ioctl_send_command {
  ioctl_command command;
};

result<ioctl_command> ioctl(raw_fd, ioctl_send_command) noexcept;

enum class ioctl_write_option : int {
  dont_send_zero_length_message = 0,
  send_zero_length_message = SNDZERO
};

struct ioctl_set_write_option {
  ioctl_write_option write_option;
};

result<ioctl_result> ioctl(raw_fd, ioctl_set_write_option) noexcept;

struct {
} ioctl_get_write_option;

result<ioctl_write_option> ioctl(raw_fd, decltype(ioctl_get_write_option)) noexcept;

struct ioctl_send_file_descriptor {
  raw_fd file_descriptor;
};

result<ioctl_result> ioctl(raw_fd, ioctl_send_file_descriptor) noexcept;

struct {
} ioctl_receive_file_descriptor;

class ioctl_new_file_descriptor {
 public:
  constexpr ioctl_new_file_descriptor(raw_fd fd, group_id gid, user_id uid) noexcept {
    new_fd_.fd = fd.raw();
    new_fd_.gid = underlying_value(gid);
    new_fd_.uid = underlying_value(uid);
  }

  constexpr ioctl_new_file_descriptor(const ::strrecvfd &new_fd) : new_fd_{new_fd} {}

  constexpr raw_fd fd() const noexcept { return raw_fd{new_fd_.fd}; }
  constexpr group_id group() const noexcept { return group_id{new_fd_.gid}; }
  constexpr user_id user() const noexcept { return user_id{new_fd_.uid}; }

 private:
  ::strrecvfd new_fd_{};
};

result<ioctl_new_file_descriptor> ioctl(raw_fd, decltype(ioctl_receive_file_descriptor)) noexcept;

struct {
} ioctl_get_module_count;

result<ioctl_result> ioctl(raw_fd, decltype(ioctl_get_module_count)) noexcept;

struct ioctl_get_module_names {
  int num_modules{};
};

class ioctl_module_name_list {
 public:
  ioctl_module_name_list(std::unique_ptr<::str_mlist[]>, unsigned) noexcept;

  std::string_view at(std::size_t) const noexcept(false);

 private:
  std::unique_ptr<::str_mlist[]> module_names_{};
  unsigned module_name_count_{};
};

result<ioctl_module_name_list> ioctl(raw_fd, ioctl_get_module_names) noexcept(false);

}