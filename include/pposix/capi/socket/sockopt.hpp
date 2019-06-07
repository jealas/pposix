#pragma once

#include <system_error>

#include <sys/socket.h>

#include "pposix/capi/socket/level.hpp"
#include "pposix/capi/socket/option.hpp"
#include "pposix/capi/socket/socketfd.hpp"
#include "pposix/duration.hpp"
#include "pposix/errno_code.hpp"
#include "pposix/util/underlying_value.hpp"

namespace pposix::capi::socket {

enum class debug : bool {};
enum class broadcast : bool {};
enum class reuseaddr : bool {};
enum class keepalive : int {};

class linger {
 public:
  linger() = default;
  linger(::linger l) : linger_{l} {}  // NOLINT implicit constructor
  explicit linger(pposix::seconds duration) : linger_{true, duration.count()} {}
  linger(bool enabled, pposix::seconds duration) : linger_{enabled, duration.count()} {}

  linger(const linger &) = default;
  linger(linger &&) = default;

  linger &operator=(const linger &) = default;
  linger &operator=(linger &&) = default;

  const ::linger &get() const noexcept { return linger_; }

 private:
  ::linger linger_{};
};

enum class oobinline : bool {};
enum class sndbuf : int {};
enum class rcvbuf : int {};
enum class dontroute : bool {};
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

 private:
  ::timeval timeout_{};
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

 private:
  ::timeval timeout_{};
};

std::error_code setsockopt(socketfd fd, level l, option o, void const *val,
                           socklen_t len) noexcept {
  const auto error =
      ::setsockopt(fd.fd(), util::underlying_value(l), util::underlying_value(o), val, len);

  if (error == -1) {
    return errno_code();
  } else {
    return {};
  }
}

namespace detail {

std::error_code setsockopt_int(socketfd fd, level l, option o, int i) noexcept {
  return socket::setsockopt(fd, l, o, &i, sizeof(int));
}

std::error_code setsockopt_bool(socketfd fd, level l, option o, bool b) noexcept {
  return detail::setsockopt_int(fd, l, o, b ? 1 : 0);
}

}  // namespace detail

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
  return detail::setsockopt_int(fd, l, option::keepalive, util::underlying_value(k));
}

std::error_code setsockopt(socketfd fd, level l, linger lin) noexcept {
  const ::linger &posix_linger = lin.get();
  return setsockopt(fd, l, option::linger, &posix_linger, sizeof(::linger));
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
  return setsockopt(fd, l, option::rcvtimeo, &val, sizeof(::timeval));
}

std::error_code setsockopt(socketfd fd, level l, sndlowat s) noexcept {
  return detail::setsockopt_int(fd, l, option::sndlowat, util::underlying_value(s));
}

std::error_code setsockopt(socketfd fd, level l, sndtimeo r) noexcept {
  const ::timeval &val = r.get();
  return setsockopt(fd, l, option::sndtimeo, &val, sizeof(::timeval));
}

}