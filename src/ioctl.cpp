#include "pposix/ioctl.hpp"

#include <stdexcept>
#include <string>

#include "pposix/errno.hpp"
#include "pposix/util.hpp"

namespace pposix {

result<ioctl_result> ioctl(raw_fd fd, ioctl_request r, int i) noexcept {
  if (const auto res{::ioctl(fd.raw(), underlying_value(r), i)}; res == -1) {
    return current_errno_code();
  } else {
    return res;
  }
}

result<ioctl_result> ioctl(raw_fd fd, ioctl_request r, void *ptr) noexcept {
  if (const auto res{::ioctl(fd.raw(), underlying_value(r), ptr)}; res == -1) {
    return current_errno_code();
  } else {
    return res;
  }
}

result<ioctl_result> ioctl(raw_fd fd, ioctl_request r, const void *ptr) noexcept {
  if (const auto res{::ioctl(fd.raw(), underlying_value(r), ptr)}; res == -1) {
    return current_errno_code();
  } else {
    return res;
  }
}

result<ioctl_result> ioctl(raw_fd fd, ioctl_request r, nullptr_t) noexcept {
  void *ptr = nullptr;
  return pposix::ioctl(fd, r, ptr);
}

result<ioctl_result> ioctl(raw_fd fd, ioctl_request r, any_span val) noexcept {
  return pposix::ioctl(fd, r, val.data());
}

result<ioctl_result> ioctl(raw_fd fd, ioctl_request r, any_cspan val) noexcept {
  return pposix::ioctl(fd, r, val.data());
}

result<ioctl_result> ioctl(raw_fd fd, ioctl_push p) noexcept {
  return pposix::ioctl(fd, ioctl_request{I_PUSH}, p.name);
}

result<ioctl_result> ioctl(raw_fd fd, decltype(ioctl_pop)) noexcept {
  return pposix::ioctl(fd, ioctl_request{I_POP}, 0);
}

result<ioctl_result> ioctl(raw_fd fd, ioctl_look l) noexcept {
  return pposix::ioctl(fd, ioctl_request{I_LOOK}, l.name);
}

result<ioctl_result> ioctl(raw_fd fd, ioctl_flush f) noexcept {
  return pposix::ioctl(fd, ioctl_request{I_FLUSH}, underlying_value(f));
}

result<ioctl_result> ioctl(raw_fd fd, ioctl_flush_band b) noexcept {
  ::bandinfo info = b;
  return pposix::ioctl(fd, ioctl_request{I_FLUSHBAND}, &info);
}

result<ioctl_result> ioctl(raw_fd fd, ioctl_set_signal s) noexcept {
  return pposix::ioctl(fd, ioctl_request{I_SETSIG}, underlying_value(s.signals));
}

result<ioctl_signal> ioctl(raw_fd fd, decltype(ioctl_get_signal)) noexcept {
  int arg{};

  const auto result = pposix::ioctl(fd, ioctl_request{I_GETSIG}, &arg);
  if (const auto error{result.error()}) {
    return error;
  } else {
    return ioctl_signal{arg};
  }
}

result<bool> ioctl(raw_fd fd, ioctl_find f) noexcept {
  const auto res{pposix::ioctl(fd, ioctl_request{I_FIND}, f.name)};
  if (const auto value{res.value()}) {
    return static_cast<bool>(*value);
  } else {
    return res.error();
  }
}

// TODO: Finish implementing correctly.
result<strpeek> ioctl(raw_fd fd, decltype(ioctl_peek)) noexcept {
  strpeek peek{};

  const auto res{pposix::ioctl(fd, ioctl_request{I_PEEK}, &peek)};
  if (const auto error{res.error()}) {
    return error;
  } else {
    return peek;
  }
}

result<ioctl_result> ioctl(raw_fd fd, ioctl_set_read_option o) noexcept {
  ioctl_read_option &read_option = o.read_option;

  return pposix::ioctl(fd, ioctl_request{I_SRDOPT}, o.read_option.raw_read_option());
}

result<ioctl_read_option> ioctl(raw_fd fd, decltype(ioctl_get_read_option)) noexcept {
  int option{};
  const auto res{pposix::ioctl(fd, ioctl_request{I_GRDOPT}, &option)};

  if (const auto error{res.error()}) {
    return error;
  } else {
    return ioctl_read_option{option};
  }
}

result<next_message_count> ioctl(raw_fd fd, decltype(ioctl_get_next_message_count)) noexcept {
  int next_message_length{};
  const auto res{pposix::ioctl(fd, ioctl_request{I_NREAD}, &next_message_length)};

  if (const auto error{res.error()}) {
    return error;
  } else {
    return next_message_count{next_message_length, *res.value()};
  }
}

result<ioctl_command> ioctl(raw_fd fd, ioctl_send_command send_command) noexcept {
  ::strioctl ioctl_cmd(send_command.command);

  const auto res{pposix::ioctl(fd, ioctl_request{I_STR}, &ioctl_cmd)};
  if (const auto error{res.error()}) {
    return error;
  } else {
    return ioctl_command{ioctl_request{ioctl_cmd.ic_cmd}, seconds{ioctl_cmd.ic_timout},
                         any_span{ioctl_cmd.ic_dp, static_cast<size_t>(ioctl_cmd.ic_len)}};
  }
}

result<ioctl_result> ioctl(raw_fd fd, ioctl_set_write_option write_option) noexcept {
  const auto res{
      pposix::ioctl(fd, ioctl_request{I_SWROPT}, underlying_value(write_option.write_option))};
  return res.error();
}

result<ioctl_write_option> ioctl(raw_fd fd, decltype(ioctl_get_write_option)) noexcept {
  int write_option{};

  if (const auto res{pposix::ioctl(fd, ioctl_request{I_GWROPT}, &write_option)}; res.error()) {
    return res.error();
  } else {
    return ioctl_write_option{write_option};
  }
}

result<ioctl_result> ioctl(raw_fd fd, ioctl_send_file_descriptor send_fd) noexcept {
  return pposix::ioctl(fd, ioctl_request{I_SENDFD}, send_fd.file_descriptor.raw());
}

result<ioctl_new_file_descriptor> ioctl(raw_fd fd,
                                        decltype(ioctl_receive_file_descriptor)) noexcept {
  ::strrecvfd new_fd{};

  if (const auto res{pposix::ioctl(fd, ioctl_request{I_RECVFD}, &new_fd)}; res.error()) {
    return res.error();
  } else {
    return ioctl_new_file_descriptor{new_fd};
  }
}

result<ioctl_result> ioctl(raw_fd fd, decltype(ioctl_get_module_count)) noexcept {
  return pposix::ioctl(fd, ioctl_request{I_LIST}, nullptr);
}

ioctl_module_name_list::ioctl_module_name_list(std::unique_ptr<::str_mlist[]> module_name_array,
                                               unsigned module_name_count) noexcept
    : module_names_{std::move(module_name_array)}, module_name_count_{module_name_count} {}

std::string_view ioctl_module_name_list::at(std::size_t index) const {
  if (index >= module_name_count_) {
    throw std::out_of_range{"The requested index is out of range. Expected index < " +
                            std::to_string(module_name_count_) +
                            " but got: " + std::to_string(index)};
  } else {
    return std::string_view{module_names_[index].l_name};
  }
}

result<ioctl_module_name_list> ioctl(raw_fd fd,
                                     ioctl_get_module_names module_names) noexcept(false) {
  ::str_list module_name_list{};

  auto name_list{std::make_unique<::str_mlist[]>(module_names.num_modules)};

  module_name_list.sl_modlist = name_list.get();
  module_name_list.sl_nmods = module_names.num_modules;

  if (const auto res{pposix::ioctl(fd, ioctl_request{I_LIST}, &module_name_list)}; res.error()) {
    return res.error();
  } else {
    return ioctl_module_name_list{std::move(name_list),
                                  static_cast<unsigned>(module_name_list.sl_nmods)};
  }
}

}  // namespace pposix
