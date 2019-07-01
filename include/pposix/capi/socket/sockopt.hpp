#pragma once

#include <limits>
#include <system_error>

#include <sys/socket.h>

#include "pposix/any_span.hpp"
#include "pposix/capi/socket/level.hpp"
#include "pposix/capi/socket/option.hpp"
#include "pposix/capi/socket/socketfd.hpp"
#include "pposix/capi/socket/type.hpp"
#include "pposix/duration.hpp"
#include "pposix/errno.hpp"
#include "pposix/result.hpp"
#include "pposix/util/underlying_value.hpp"

namespace pposix::capi::socket {

enum class debug : bool { off = false, on = true };
enum class broadcast : bool { off = false, on = true };
enum class reuseaddr : bool { off = false, on = true };
enum class keepalive : bool { off = false, on = true };

class linger {
 public:
  linger() = default;
  constexpr linger(::linger l) noexcept : linger_{l} {}  // NOLINT implicit constructor
  constexpr explicit linger(pposix::seconds duration) noexcept : linger_{true, duration.count()} {}
  linger(bool enabled, pposix::seconds duration) noexcept : linger_{enabled, duration.count()} {}

  linger(const linger &) = default;
  linger(linger &&) = default;

  linger &operator=(const linger &) = default;
  linger &operator=(linger &&) = default;

  constexpr const ::linger &get() const noexcept { return linger_; }

  constexpr bool enabled() const noexcept { return linger_.l_onoff; }
  pposix::seconds duration() const noexcept { return pposix::seconds{linger_.l_linger}; }

 private:
  ::linger linger_;
};

enum class oobinline : bool { off = false, on = true };
enum class sndbuf : int {};
enum class rcvbuf : int {};
enum class dontroute : bool { off = false, on = true };
enum class rcvlowat : int {};

class rcvtimeo {
 public:
  rcvtimeo() = default;

  explicit rcvtimeo(pposix::seconds s) : timeout_{s.count(), 0u} {}
  rcvtimeo(pposix::seconds s, pposix::microseconds us) : timeout_{s.count(), us.count()} {}

  rcvtimeo(const rcvtimeo &) = default;
  rcvtimeo(rcvtimeo &&) = default;

  rcvtimeo &operator=(const rcvtimeo &) = default;
  rcvtimeo &operator=(rcvtimeo &&) = default;

  const ::timeval &get() const noexcept { return timeout_; }

  constexpr pposix::seconds seconds() const noexcept { return pposix::seconds{timeout_.tv_sec}; }

  constexpr pposix::microseconds microseconds() const noexcept {
    return pposix::microseconds{timeout_.tv_usec};
  }

 private:
  ::timeval timeout_;
};

enum class sndlowat : int {};

class sndtimeo {
 public:
  sndtimeo() = default;

  explicit sndtimeo(pposix::seconds s) : timeout_{s.count(), 0u} {}
  sndtimeo(pposix::seconds s, pposix::microseconds us) : timeout_{s.count(), us.count()} {}

  sndtimeo(const sndtimeo &) = default;
  sndtimeo(sndtimeo &&) = default;

  sndtimeo &operator=(const sndtimeo &) = default;
  sndtimeo &operator=(sndtimeo &&) = default;

  const ::timeval &get() const noexcept { return timeout_; }

  constexpr pposix::seconds seconds() const noexcept { return pposix::seconds{timeout_.tv_sec}; }

  constexpr pposix::microseconds microseconds() const noexcept {
    return pposix::microseconds{timeout_.tv_usec};
  }

 private:
  ::timeval timeout_;
};

std::error_code setsockopt(socketfd fd, level l, option o, any_cspan val) noexcept {
  const auto error = ::setsockopt(fd.fd(), util::underlying_value(l), util::underlying_value(o),
                                  val.data(), val.length());

  if (error == -1) {
    return current_errno_code();
  } else {
    return {};
  }
}

namespace detail {

std::error_code setsockopt_int(socketfd fd, level l, option o, int i) noexcept {
  return socket::setsockopt(fd, l, o, any_cspan{i});
}

std::error_code setsockopt_bool(socketfd fd, level l, option o, bool b) noexcept {
  return detail::setsockopt_int(fd, l, o, b ? 1 : 0);
}

}  // namespace detail

template <class>
constexpr bool valid_overload = false;

template <class T>
std::error_code setsockopt(socketfd, level, debug) noexcept {
  static_assert(valid_overload<T>,
                "setsockopt not specialized for given type. Use setsockopt(socketfd, level, "
                "debug, option, any_cspan) overload instead.");

  return {};
}

std::error_code setsockopt(socketfd fd, level l, debug d) noexcept {
  return detail::setsockopt_bool(fd, l, option::debug, util::underlying_value(d));
}

std::error_code setsockopt(socketfd fd, level l, broadcast b) noexcept {
  return detail::setsockopt_bool(fd, l, option::broadcast, util::underlying_value(b));
}

std::error_code setsockopt(socketfd fd, level l, reuseaddr r) noexcept {
  return detail::setsockopt_bool(fd, l, option::reuseaddr, util::underlying_value(r));
}

std::error_code setsockopt(socketfd fd, level l, keepalive k) noexcept {
  return detail::setsockopt_bool(fd, l, option::keepalive, util::underlying_value(k));
}

std::error_code setsockopt(socketfd fd, level l, linger lin) noexcept {
  const ::linger &posix_linger = lin.get();
  return setsockopt(fd, l, option::linger, any_cspan{posix_linger});
}

std::error_code setsockopt(socketfd fd, level l, oobinline o) noexcept {
  return detail::setsockopt_bool(fd, l, option::oobinline, util::underlying_value(o));
}

std::error_code setsockopt(socketfd fd, level l, sndbuf s) noexcept {
  return detail::setsockopt_int(fd, l, option::sndbuf, util::underlying_value(s));
}

std::error_code setsockopt(socketfd fd, level l, rcvbuf r) noexcept {
  return detail::setsockopt_int(fd, l, option::rcvbuf, util::underlying_value(r));
}

std::error_code setsockopt(socketfd fd, level l, dontroute d) noexcept {
  return detail::setsockopt_bool(fd, l, option::dontroute, util::underlying_value(d));
}

std::error_code setsockopt(socketfd fd, level l, rcvlowat r) noexcept {
  return detail::setsockopt_int(fd, l, option::rcvlowat, util::underlying_value(r));
}

std::error_code setsockopt(socketfd fd, level l, rcvtimeo r) noexcept {
  const ::timeval &val = r.get();
  return setsockopt(fd, l, option::rcvtimeo, any_cspan{val});
}

std::error_code setsockopt(socketfd fd, level l, sndlowat s) noexcept {
  return detail::setsockopt_int(fd, l, option::sndlowat, util::underlying_value(s));
}

std::error_code setsockopt(socketfd fd, level l, sndtimeo r) noexcept {
  const ::timeval &val = r.get();
  return setsockopt(fd, l, option::sndtimeo, any_cspan{val});
}

result<socklen_t> getsockopt(socketfd fd, level l, option o, any_span val) noexcept {
  if (val.length() > std::numeric_limits<socklen_t>::max()) {
    return make_errno_code(std::errc::value_too_large);
  }

  socklen_t len{static_cast<socklen_t>(val.length())};

  const auto error = ::getsockopt(fd.fd(), util::underlying_value(l), util::underlying_value(o),
                                  val.data(), &len);

  if (error == -1) {
    return current_errno_code();
  } else {
    return len;
  }
}

namespace detail {

template <class Result, option Option>
result<Result> getsockopt_int(socketfd fd, level l) noexcept {
  int val{};

  const auto result = socket::getsockopt(fd, l, Option, any_span{val});
  if (const auto error = result.error()) {
    return error;
  } else {
    return Result{val};
  }
}

template <class Result, option Option>
result<Result> getsockopt_bool(socketfd fd, level l) noexcept {
  int val{};

  const auto result = socket::getsockopt(fd, l, Option, any_span{val});
  if (const auto error = result.error()) {
    return error;
  } else {
    return Result{val != 0};
  }
}

template <class Result, option Option>
result<Result> getsockopt_struct(socketfd fd, level l) noexcept {
  static_assert(std::is_class_v<Result>);

  Result val{};

  const auto result = socket::getsockopt(fd, l, Option, any_span{val});
  if (const auto error = result.error()) {
    return error;
  } else {
    return val;
  }
}

}  // namespace detail

template <class T>
result<T> getsockopt(socketfd fd, level l) noexcept {
  static_assert(valid_overload<T>,
                "getsockopt not specialized for given type. Use getsockopt(socketfd, level, "
                "option, any_span) overload instead.");
}

template <>
result<debug> getsockopt<debug>(socketfd fd, level l) noexcept {
  return detail::getsockopt_bool<debug, option::debug>(fd, l);
}

template <>
result<broadcast> getsockopt<broadcast>(socketfd fd, level l) noexcept {
  return detail::getsockopt_bool<broadcast, option::broadcast>(fd, l);
}

template <>
result<reuseaddr> getsockopt<reuseaddr>(socketfd fd, level l) noexcept {
  return detail::getsockopt_bool<reuseaddr, option::reuseaddr>(fd, l);
}

enum class acceptconn : bool {};

template <>
result<acceptconn> getsockopt<acceptconn>(socketfd fd, level l) noexcept {
  return detail::getsockopt_bool<acceptconn, option::acceptconn>(fd, l);
}

template <>
result<keepalive> getsockopt<keepalive>(socketfd fd, level l) noexcept {
  return detail::getsockopt_bool<keepalive, option::keepalive>(fd, l);
}

template <>
result<linger> getsockopt<linger>(socketfd fd, level l) noexcept {
  return detail::getsockopt_struct<linger, option::linger>(fd, l);
}

template <>
result<oobinline> getsockopt<oobinline>(socketfd fd, level l) noexcept {
  return detail::getsockopt_bool<oobinline, option::oobinline>(fd, l);
}

template <>
result<sndbuf> getsockopt<sndbuf>(socketfd fd, level l) noexcept {
  return detail::getsockopt_int<sndbuf, option::sndbuf>(fd, l);
}

template <>
result<rcvbuf> getsockopt<rcvbuf>(socketfd fd, level l) noexcept {
  return detail::getsockopt_int<rcvbuf, option::rcvbuf>(fd, l);
}

enum class error : int {};

template <>
result<error> getsockopt<error>(socketfd fd, level l) noexcept {
  return detail::getsockopt_int<error, option::error>(fd, l);
}

template <>
result<type> getsockopt<type>(socketfd fd, level l) noexcept {
  return detail::getsockopt_int<type, option::type>(fd, l);
}

template <>
result<dontroute> getsockopt<dontroute>(socketfd fd, level l) noexcept {
  return detail::getsockopt_bool<dontroute, option::dontroute>(fd, l);
}

template <>
result<rcvlowat> getsockopt<rcvlowat>(socketfd fd, level l) noexcept {
  return detail::getsockopt_int<rcvlowat, option::rcvlowat>(fd, l);
}

template <>
result<rcvtimeo> getsockopt<rcvtimeo>(socketfd fd, level l) noexcept {
  return detail::getsockopt_struct<rcvtimeo, option::rcvtimeo>(fd, l);
}

template <>
result<sndlowat> getsockopt<sndlowat>(socketfd fd, level l) noexcept {
  return detail::getsockopt_int<sndlowat, option::sndlowat>(fd, l);
}

template <>
result<sndtimeo> getsockopt<sndtimeo>(socketfd fd, level l) noexcept {
  return detail::getsockopt_struct<sndtimeo, option::sndtimeo>(fd, l);
}

}