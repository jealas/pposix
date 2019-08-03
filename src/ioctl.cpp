#include "pposix/ioctl.hpp"

#include <stdexcept>
#include <string>

#include "pposix/errno.hpp"
#include "pposix/util.hpp"

namespace pposix {

result<ioctl_result> ioctl(const raw_fd &fd, ioctl_request r, int i) noexcept {
  if (const auto res{::ioctl(fd.raw(), underlying_value(r), i)}; res == -1) {
    return current_errno_code();
  } else {
    return res;
  }
}

result<ioctl_result> ioctl(const raw_fd &fd, ioctl_request r, void *ptr) noexcept {
  if (const auto res{::ioctl(fd.raw(), underlying_value(r), ptr)}; res == -1) {
    return current_errno_code();
  } else {
    return res;
  }
}

result<ioctl_result> ioctl(const raw_fd &fd, ioctl_request r, const void *ptr) noexcept {
  if (const auto res{::ioctl(fd.raw(), underlying_value(r), ptr)}; res == -1) {
    return current_errno_code();
  } else {
    return res;
  }
}

result<ioctl_result> ioctl(const raw_fd &fd, ioctl_request r, nullptr_t) noexcept {
  void *ptr = nullptr;
  return pposix::ioctl(fd, r, ptr);
}

result<ioctl_result> ioctl(const raw_fd &fd, ioctl_request r, any_span val) noexcept {
  return pposix::ioctl(fd, r, val.data());
}

result<ioctl_result> ioctl(const raw_fd &fd, ioctl_request r, any_cspan val) noexcept {
  return pposix::ioctl(fd, r, val.data());
}

result<ioctl_result> ioctl(const raw_fd &fd, ioctl_push p) noexcept {
  return pposix::ioctl(fd, ioctl_request::push, p.name);
}

result<ioctl_result> ioctl(const raw_fd &fd, decltype(ioctl_pop)) noexcept {
  return pposix::ioctl(fd, ioctl_request::pop, 0);
}

result<ioctl_result> ioctl(const raw_fd &fd, ioctl_look l) noexcept {
  return pposix::ioctl(fd, ioctl_request::look, l.name);
}

result<ioctl_result> ioctl(const raw_fd &fd, ioctl_flush f) noexcept {
  return pposix::ioctl(fd, ioctl_request::flush, underlying_value(f));
}

result<ioctl_result> ioctl(const raw_fd &fd, ioctl_flush_band b) noexcept {
  ::bandinfo info = b;
  return pposix::ioctl(fd, ioctl_request::flush_band, &info);
}

result<ioctl_result> ioctl(const raw_fd &fd, ioctl_set_signal s) noexcept {
  return pposix::ioctl(fd, ioctl_request::set_signal, underlying_value(s.signals));
}

result<ioctl_signal> ioctl(const raw_fd &fd, decltype(ioctl_get_signal)) noexcept {
  int arg{};

  const auto result = pposix::ioctl(fd, ioctl_request::get_signal, &arg);
  return result_map<ioctl_signal>(result,
                                  [&](ioctl_result /*ignored*/) { return ioctl_signal{arg}; });
}

result<bool> ioctl(const raw_fd &fd, ioctl_find f) noexcept {
  const auto res{pposix::ioctl(fd, ioctl_request::find, f.name)};
  return result_map<bool>(res, [](ioctl_result found) { return found != 0; });
}

// TODO: Finish implementing correctly.
result<strpeek> ioctl(const raw_fd &fd, decltype(ioctl_peek)) noexcept {
  strpeek peek{};

  const auto res{pposix::ioctl(fd, ioctl_request::peek, &peek)};
  return result_map<strpeek>(res, [&](ioctl_result /*ignored*/) { return peek; });
}

result<ioctl_result> ioctl(const raw_fd &fd, ioctl_set_read_option o) noexcept {
  ioctl_read_option &read_option = o.read_option;

  return pposix::ioctl(fd, ioctl_request::set_read_option, o.read_option.raw_read_option());
}

result<ioctl_read_option> ioctl(const raw_fd &fd, decltype(ioctl_get_read_option)) noexcept {
  int option{};
  const auto res{pposix::ioctl(fd, ioctl_request::get_read_option, &option)};
  return result_map<ioctl_read_option>(
      res, [&](ioctl_result /*ignored*/) { return ioctl_read_option{option}; });
}

result<next_message_count> ioctl(const raw_fd &fd,
                                 decltype(ioctl_get_next_message_count)) noexcept {
  int next_message_length{};
  const auto res{pposix::ioctl(fd, ioctl_request::nread, &next_message_length)};
  return result_map<next_message_count>(res, [&](ioctl_result num_messages) {
    return next_message_count{next_message_length, num_messages};
  });
}

result<ioctl_command> ioctl(const raw_fd &fd, ioctl_send_command send_command) noexcept {
  ::strioctl ioctl_cmd(send_command.command);

  const auto res{pposix::ioctl(fd, ioctl_request::send_command, &ioctl_cmd)};
  return result_map<ioctl_command>(res, [&](ioctl_result /*ignored*/) {
    return ioctl_command{ioctl_request{ioctl_cmd.ic_cmd}, seconds{ioctl_cmd.ic_timout},
                         any_span{ioctl_cmd.ic_dp, static_cast<size_t>(ioctl_cmd.ic_len)}};
  });
}

result<ioctl_result> ioctl(const raw_fd &fd, ioctl_set_write_option write_option) noexcept {
  return pposix::ioctl(fd, ioctl_request::set_write_option,
                       underlying_value(write_option.write_option));
}

result<ioctl_write_option> ioctl(const raw_fd &fd, decltype(ioctl_get_write_option)) noexcept {
  int write_option{};

  const auto res{pposix::ioctl(fd, ioctl_request::get_write_option, &write_option)};
  return result_map<ioctl_write_option>(
      res, [&](ioctl_result /*ignored*/) { return ioctl_write_option{write_option}; });
}

result<ioctl_result> ioctl(const raw_fd &fd, ioctl_send_file_descriptor send_fd) noexcept {
  return pposix::ioctl(fd, ioctl_request::send_file_descriptor, send_fd.file_descriptor.raw());
}

result<ioctl_new_file_descriptor> ioctl(const raw_fd &fd,
                                        decltype(ioctl_receive_file_descriptor)) noexcept {
  ::strrecvfd new_fd{};
  const auto res{pposix::ioctl(fd, ioctl_request::receive_file_descriptor, &new_fd)};
  return result_map<ioctl_new_file_descriptor>(
      res, [&](ioctl_result /*ignored*/) { return ioctl_new_file_descriptor{new_fd}; });
}

result<ioctl_result> ioctl(const raw_fd &fd, decltype(ioctl_get_module_count)) noexcept {
  return pposix::ioctl(fd, ioctl_request::list_module_names, nullptr);
}

ioctl_module_name_list::ioctl_module_name_list(std::unique_ptr<::str_mlist[]> module_name_array,
                                               unsigned module_name_count) noexcept
    : module_names_{std::move(module_name_array)}, module_name_count_{module_name_count} {}

std::string_view ioctl_module_name_list::at(std::size_t index) const noexcept(false) {
  if (index >= module_name_count_) {
    throw std::out_of_range{"The requested index is out of range. Expected index < " +
                            std::to_string(module_name_count_) +
                            " but got: " + std::to_string(index)};
  } else {
    return std::string_view{module_names_[index].l_name};
  }
}

result<ioctl_module_name_list> ioctl(const raw_fd &fd,
                                     ioctl_get_module_names module_names) noexcept(false) {
  ::str_list module_name_list{};

  auto name_list{std::make_unique<::str_mlist[]>(module_names.num_modules)};

  module_name_list.sl_modlist = name_list.get();
  module_name_list.sl_nmods = module_names.num_modules;

  const auto res{pposix::ioctl(fd, ioctl_request::list_module_names, &module_name_list)};
  return result_map<ioctl_module_name_list>(res, [&](ioctl_result /*ignored*/) {
    return ioctl_module_name_list{std::move(name_list),
                                  static_cast<unsigned>(module_name_list.sl_nmods)};
  });
}

}  // namespace pposix
