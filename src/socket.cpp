#include "pposix/socket.hpp"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "pposix/any_span.hpp"
#include "pposix/duration.hpp"
#include "pposix/fd.hpp"
#include "pposix/result.hpp"
#include "pposix/unique_fd.hpp"
#include "pposix/util.hpp"

namespace pposix {

socket_flag operator|(const socket_flag &lhs, const socket_flag &rhs) noexcept {
  return socket_flag{underlying_value(lhs) | underlying_value(rhs)};
}

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
std::error_code setsockopt(socket_fd fd, socket_level l, socket_option o, any_cspan val) noexcept {
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
  return pposix::setsockopt(fd, l, o, any_cspan{i});
}

std::error_code setsockopt_bool(socket_fd fd, socket_level l, socket_option o, bool b) noexcept {
  return setsockopt_int(fd, l, o, b ? 1 : 0);
}

}  // namespace

std::error_code setsockopt(socket_fd fd, socket_level l, debug d) noexcept {
  return setsockopt_bool(fd, l, socket_option::debug, underlying_value(d));
}

std::error_code setsockopt(socket_fd fd, socket_level l, broadcast b) noexcept {
  return setsockopt_bool(fd, l, socket_option::broadcast, underlying_value(b));
}

std::error_code setsockopt(socket_fd fd, socket_level l, reuseaddr r) noexcept {
  return setsockopt_bool(fd, l, socket_option::reuseaddr, underlying_value(r));
}

std::error_code setsockopt(socket_fd fd, socket_level l, keepalive k) noexcept {
  return setsockopt_bool(fd, l, socket_option::keepalive, underlying_value(k));
}

std::error_code setsockopt(socket_fd fd, socket_level l, linger lin) noexcept {
  const ::linger &posix_linger = lin.get();
  return setsockopt(fd, l, socket_option::linger, any_cspan{posix_linger});
}

std::error_code setsockopt(socket_fd fd, socket_level l, oobinline o) noexcept {
  return setsockopt_bool(fd, l, socket_option::oobinline, underlying_value(o));
}

std::error_code setsockopt(socket_fd fd, socket_level l, sndbuf s) noexcept {
  return setsockopt_int(fd, l, socket_option::sndbuf, underlying_value(s));
}

std::error_code setsockopt(socket_fd fd, socket_level l, rcvbuf r) noexcept {
  return setsockopt_int(fd, l, socket_option::rcvbuf, underlying_value(r));
}

std::error_code setsockopt(socket_fd fd, socket_level l, dontroute d) noexcept {
  return setsockopt_bool(fd, l, socket_option::dontroute, underlying_value(d));
}

std::error_code setsockopt(socket_fd fd, socket_level l, rcvlowat r) noexcept {
  return setsockopt_int(fd, l, socket_option::rcvlowat, underlying_value(r));
}

std::error_code setsockopt(socket_fd fd, socket_level l, rcvtimeo r) noexcept {
  const ::timeval &val = r.get();
  return setsockopt(fd, l, socket_option::rcvtimeo, any_cspan{val});
}

std::error_code setsockopt(socket_fd fd, socket_level l, sndlowat s) noexcept {
  return setsockopt_int(fd, l, socket_option::sndlowat, underlying_value(s));
}

std::error_code setsockopt(socket_fd fd, socket_level l, sndtimeo r) noexcept {
  const ::timeval &val = r.get();
  return setsockopt(fd, l, socket_option::sndtimeo, any_cspan{val});
}

// Get socket option
result<socklen_t> getsockopt(socket_fd fd, socket_level l, socket_option o,
                             any_span val) noexcept {
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

  const auto result = pposix::getsockopt(fd, l, o, any_span{val});
  return result_map<Result>(result, [&](int /*ignored*/) { return Result{val}; });
}

template <class Result>
result<Result> getsockopt_bool(socket_fd fd, socket_level l, socket_option o) noexcept {
  int val{};

  const auto result = pposix::getsockopt(fd, l, o, any_span{val});
  return result_map<Result>(result, [&](int /*ignored*/) { return Result{val != 0}; });
}

template <class Result>
result<Result> getsockopt_struct(socket_fd fd, socket_level l, socket_option o) noexcept {
  static_assert(std::is_class_v<Result>);

  Result val{};

  const auto result = pposix::getsockopt(fd, l, o, any_span{val});
  return result_map<Result>(result, [&](int /*ignored*/) { return val; });
}

}  // namespace detail

template <>
result<debug> getsockopt<debug>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_bool<debug>(fd, l, socket_option::debug);
}

template <>
result<broadcast> getsockopt<broadcast>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_bool<broadcast>(fd, l, socket_option::broadcast);
}

template <>
result<reuseaddr> getsockopt<reuseaddr>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_bool<reuseaddr>(fd, l, socket_option::reuseaddr);
}

template <>
result<acceptconn> getsockopt<acceptconn>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_bool<acceptconn>(fd, l, socket_option::acceptconn);
}

template <>
result<keepalive> getsockopt<keepalive>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_bool<keepalive>(fd, l, socket_option::keepalive);
}

template <>
result<linger> getsockopt<linger>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_struct<linger>(fd, l, socket_option::linger);
}

template <>
result<oobinline> getsockopt<oobinline>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_bool<oobinline>(fd, l, socket_option::oobinline);
}

template <>
result<sndbuf> getsockopt<sndbuf>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_int<sndbuf>(fd, l, socket_option::sndbuf);
}

template <>
result<rcvbuf> getsockopt<rcvbuf>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_int<rcvbuf>(fd, l, socket_option::rcvbuf);
}

template <>
result<error> getsockopt<error>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_int<error>(fd, l, socket_option::error);
}

template <>
result<socket_type> getsockopt<socket_type>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_int<socket_type>(fd, l, socket_option::type);
}

template <>
result<dontroute> getsockopt<dontroute>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_bool<dontroute>(fd, l, socket_option::dontroute);
}

template <>
result<rcvlowat> getsockopt<rcvlowat>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_int<rcvlowat>(fd, l, socket_option::rcvlowat);
}

template <>
result<rcvtimeo> getsockopt<rcvtimeo>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_struct<rcvtimeo>(fd, l, socket_option::rcvtimeo);
}

template <>
result<sndlowat> getsockopt<sndlowat>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_int<sndlowat>(fd, l, socket_option::sndlowat);
}

template <>
result<sndtimeo> getsockopt<sndtimeo>(socket_fd fd, socket_level l) noexcept {
  return detail::getsockopt_struct<sndtimeo>(fd, l, socket_option::sndtimeo);
}

}  // namespace pposix
