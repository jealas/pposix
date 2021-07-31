#include "pposix/socket.hpp"

#include <sys/socket.h>

#include "pposix/any_view.hpp"
#include "pposix/duration.hpp"
#include "pposix/result.hpp"
#include "pposix/util.hpp"

namespace pposix {

namespace capi {

// Socket
result<socket_fd> socket(socket_domain dom, socket_type typ, socket_flag flags,
                         socket_protocol prot) noexcept {
  const auto sock{
      ::socket(underlying_v(dom), underlying_v(typ) | underlying_v(flags), underlying_v(prot))};
  if (sock == -1) {
    return current_errno_code();
  } else {
    return socket_fd{raw_socket_fd{sock}};
  }
}

// Set socket option
std::error_code setsockopt(raw_socket_fd fd, socket_level l, socket_option o,
                           any_cview val) noexcept {
  return PPOSIX_COMMON_CALL(::setsockopt, static_cast<socket_fd_t>(fd), underlying_v(l),
                            underlying_v(o), val.data(), val.length());
}

// Get socket option
result<socklen_t> getsockopt(raw_socket_fd fd, socket_level l, socket_option o,
                             any_view val) noexcept {
  if (val.length() > std::numeric_limits<socklen_t>::max()) {
    return make_errno_code(std::errc::value_too_large);
  }

  socklen_t len{static_cast<socklen_t>(val.length())};

  const auto error = ::getsockopt(static_cast<socket_fd_t>(fd), underlying_v(l), underlying_v(o),
                                  val.data(), &len);

  if (error == -1) {
    return current_errno_code();
  } else {
    return len;
  }
}

// Get socket option
std::error_code setsockopt_int(raw_socket_fd fd, socket_level l, socket_option o, int i) noexcept {
  return setsockopt(fd, l, o, any_cview{&i});
}

std::error_code setsockopt_bool(raw_socket_fd fd, socket_level l, socket_option o,
                                bool b) noexcept {
  return setsockopt_int(fd, l, o, b ? 1 : 0);
}

std::error_code setsockopt(raw_socket_fd fd, socket_level l, socket_debug d) noexcept {
  return setsockopt_bool(fd, l, socket_option::debug, underlying_v(d));
}

std::error_code setsockopt(raw_socket_fd fd, socket_level l, socket_broadcast b) noexcept {
  return setsockopt_bool(fd, l, socket_option::broadcast, underlying_v(b));
}

std::error_code setsockopt(raw_socket_fd fd, socket_level l, socket_reuseaddr r) noexcept {
  return setsockopt_bool(fd, l, socket_option::reuseaddr, underlying_v(r));
}

std::error_code setsockopt(raw_socket_fd fd, socket_level l, socket_keepalive k) noexcept {
  return setsockopt_bool(fd, l, socket_option::keepalive, underlying_v(k));
}

std::error_code setsockopt(raw_socket_fd fd, socket_level l, socket_linger lin) noexcept {
  const ::linger &posix_linger = lin.get();
  return setsockopt(fd, l, socket_option::linger, any_cview{&posix_linger});
}

std::error_code setsockopt(raw_socket_fd fd, socket_level l, socket_oobinline o) noexcept {
  return setsockopt_bool(fd, l, socket_option::oobinline, underlying_v(o));
}

std::error_code setsockopt(raw_socket_fd fd, socket_level l, socket_sndbuf s) noexcept {
  return setsockopt_int(fd, l, socket_option::sndbuf, underlying_v(s));
}

std::error_code setsockopt(raw_socket_fd fd, socket_level l, socket_rcvbuf r) noexcept {
  return setsockopt_int(fd, l, socket_option::rcvbuf, underlying_v(r));
}

std::error_code setsockopt(raw_socket_fd fd, socket_level l, socket_dontroute d) noexcept {
  return setsockopt_bool(fd, l, socket_option::dontroute, underlying_v(d));
}

std::error_code setsockopt(raw_socket_fd fd, socket_level l, socket_rcvlowat r) noexcept {
  return setsockopt_int(fd, l, socket_option::rcvlowat, underlying_v(r));
}

std::error_code setsockopt(raw_socket_fd fd, socket_level l, socket_rcvtimeo r) noexcept {
  const ::timeval &val = r.get();
  return setsockopt(fd, l, socket_option::rcvtimeo, any_cview{&val});
}

std::error_code setsockopt(raw_socket_fd fd, socket_level l, socket_sndlowat s) noexcept {
  return setsockopt_int(fd, l, socket_option::sndlowat, underlying_v(s));
}

std::error_code setsockopt(raw_socket_fd fd, socket_level l, socket_sndtimeo r) noexcept {
  const ::timeval &val = r.get();
  return setsockopt(fd, l, socket_option::sndtimeo, any_cview{&val});
}

}  // namespace capi

socket::socket(capi::socket_fd fd) noexcept : socket_fd_{std::move(fd)} {}

}  // namespace pposix
