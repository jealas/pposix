#include "pposix/socket.hpp"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "pposix/any_view.hpp"
#include "pposix/duration.hpp"
#include "pposix/fd.hpp"
#include "pposix/result.hpp"
#include "pposix/unique_fd.hpp"
#include "pposix/util.hpp"

namespace pposix {

// Socket
result<unique_fd<socket_fd>> socket(socket_domain dom, socket_type typ, socket_flag flags,
                                    socket_protocol prot) noexcept {
  const socket_fd sock_fd{::socket(underlying_value(dom),
                                   underlying_value(typ) | underlying_value(flags),
                                   underlying_value(prot))};
  if (sock_fd == nullfd) {
    return current_errno_code();
  } else {
    return unique_fd<socket_fd>{sock_fd};
  }
}

// Set socket option
std::error_code setsockopt(socket_fd fd, socket_level l, socket_option o, any_cview val) noexcept {
  const auto error =
      ::setsockopt(fd.raw(), underlying_value(l), underlying_value(o), val.data(), val.length());

  if (error == -1) {
    return current_errno_code();
  } else {
    return {};
  }
}

namespace {

std::error_code setsockopt_int(socket_fd fd, socket_level l, socket_option o, int i) noexcept {
  return pposix::setsockopt(fd, l, o, any_cview{&i});
}

std::error_code setsockopt_bool(socket_fd fd, socket_level l, socket_option o, bool b) noexcept {
  return setsockopt_int(fd, l, o, b ? 1 : 0);
}

}  // namespace

std::error_code setsockopt(socket_fd fd, socket_level l, socket_debug d) noexcept {
  return setsockopt_bool(fd, l, socket_option::debug, underlying_value(d));
}

std::error_code setsockopt(socket_fd fd, socket_level l, socket_broadcast b) noexcept {
  return setsockopt_bool(fd, l, socket_option::broadcast, underlying_value(b));
}

std::error_code setsockopt(socket_fd fd, socket_level l, socket_reuseaddr r) noexcept {
  return setsockopt_bool(fd, l, socket_option::reuseaddr, underlying_value(r));
}

std::error_code setsockopt(socket_fd fd, socket_level l, socket_keepalive k) noexcept {
  return setsockopt_bool(fd, l, socket_option::keepalive, underlying_value(k));
}

std::error_code setsockopt(socket_fd fd, socket_level l, socket_linger lin) noexcept {
  const ::linger &posix_linger = lin.get();
  return setsockopt(fd, l, socket_option::linger, any_cview{&posix_linger});
}

std::error_code setsockopt(socket_fd fd, socket_level l, socket_oobinline o) noexcept {
  return setsockopt_bool(fd, l, socket_option::oobinline, underlying_value(o));
}

std::error_code setsockopt(socket_fd fd, socket_level l, socket_sndbuf s) noexcept {
  return setsockopt_int(fd, l, socket_option::sndbuf, underlying_value(s));
}

std::error_code setsockopt(socket_fd fd, socket_level l, socket_rcvbuf r) noexcept {
  return setsockopt_int(fd, l, socket_option::rcvbuf, underlying_value(r));
}

std::error_code setsockopt(socket_fd fd, socket_level l, socket_dontroute d) noexcept {
  return setsockopt_bool(fd, l, socket_option::dontroute, underlying_value(d));
}

std::error_code setsockopt(socket_fd fd, socket_level l, socket_rcvlowat r) noexcept {
  return setsockopt_int(fd, l, socket_option::rcvlowat, underlying_value(r));
}

std::error_code setsockopt(socket_fd fd, socket_level l, socket_rcvtimeo r) noexcept {
  const ::timeval &val = r.get();
  return setsockopt(fd, l, socket_option::rcvtimeo, any_cview{&val});
}

std::error_code setsockopt(socket_fd fd, socket_level l, socket_sndlowat s) noexcept {
  return setsockopt_int(fd, l, socket_option::sndlowat, underlying_value(s));
}

std::error_code setsockopt(socket_fd fd, socket_level l, socket_sndtimeo r) noexcept {
  const ::timeval &val = r.get();
  return setsockopt(fd, l, socket_option::sndtimeo, any_cview{&val});
}

// Get socket option
result<socklen_t> getsockopt(socket_fd fd, socket_level l, socket_option o,
                             any_view val) noexcept {
  if (val.length() > std::numeric_limits<socklen_t>::max()) {
    return make_errno_code(std::errc::value_too_large);
  }

  socklen_t len{static_cast<socklen_t>(val.length())};

  const auto error =
      ::getsockopt(fd.raw(), underlying_value(l), underlying_value(o), val.data(), &len);

  if (error == -1) {
    return current_errno_code();
  } else {
    return len;
  }
}

namespace detail {

template <class Result>
result<Result> getsockopt_int(socket_fd fd, socket_level l, socket_option o) noexcept {
  int val{};

  const auto result = pposix::getsockopt(fd, l, o, any_view{&val});
  return result_map<Result>(result, [&](int /*ignored*/) { return Result{val}; });
}

template <class Result>
result<Result> getsockopt_bool(socket_fd fd, socket_level l, socket_option o) noexcept {
  int val{};

  const auto result = pposix::getsockopt(fd, l, o, any_view{&val});
  return result_map<Result>(result, [&](int /*ignored*/) { return Result{val != 0}; });
}

template <class Result>
result<Result> getsockopt_struct(socket_fd fd, socket_level l, socket_option o) noexcept {
  static_assert(std::is_class_v<Result>);

  Result val{};

  const auto result = pposix::getsockopt(fd, l, o, any_view{&val});
  return result_map<Result>(result, [&](int /*ignored*/) { return val; });
}

}  // namespace detail

template <>
result<socket_debug> getsockopt<socket_debug>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_bool<socket_debug>(fd, l, socket_option::debug);
}

template <>
result<socket_broadcast> getsockopt<socket_broadcast>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_bool<socket_broadcast>(fd, l, socket_option::broadcast);
}

template <>
result<socket_reuseaddr> getsockopt<socket_reuseaddr>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_bool<socket_reuseaddr>(fd, l, socket_option::reuseaddr);
}

template <>
result<socket_acceptconn> getsockopt<socket_acceptconn>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_bool<socket_acceptconn>(fd, l, socket_option::acceptconn);
}

template <>
result<socket_keepalive> getsockopt<socket_keepalive>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_bool<socket_keepalive>(fd, l, socket_option::keepalive);
}

template <>
result<socket_linger> getsockopt<socket_linger>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_struct<socket_linger>(fd, l, socket_option::linger);
}

template <>
result<socket_oobinline> getsockopt<socket_oobinline>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_bool<socket_oobinline>(fd, l, socket_option::oobinline);
}

template <>
result<socket_sndbuf> getsockopt<socket_sndbuf>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_int<socket_sndbuf>(fd, l, socket_option::sndbuf);
}

template <>
result<socket_rcvbuf> getsockopt<socket_rcvbuf>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_int<socket_rcvbuf>(fd, l, socket_option::rcvbuf);
}

template <>
result<socket_error> getsockopt<socket_error>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_int<socket_error>(fd, l, socket_option::error);
}

template <>
result<socket_type> getsockopt<socket_type>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_int<socket_type>(fd, l, socket_option::type);
}

template <>
result<socket_dontroute> getsockopt<socket_dontroute>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_bool<socket_dontroute>(fd, l, socket_option::dontroute);
}

template <>
result<socket_rcvlowat> getsockopt<socket_rcvlowat>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_int<socket_rcvlowat>(fd, l, socket_option::rcvlowat);
}

template <>
result<socket_rcvtimeo> getsockopt<socket_rcvtimeo>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_struct<socket_rcvtimeo>(fd, l, socket_option::rcvtimeo);
}

template <>
result<socket_sndlowat> getsockopt<socket_sndlowat>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_int<socket_sndlowat>(fd, l, socket_option::sndlowat);
}

template <>
result<socket_sndtimeo> getsockopt<socket_sndtimeo>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_struct<socket_sndtimeo>(fd, l, socket_option::sndtimeo);
}

}  // namespace pposix
