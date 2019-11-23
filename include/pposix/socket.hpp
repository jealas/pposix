#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "pposix/any_view.hpp"
#include "pposix/duration.hpp"
#include "pposix/extension.hpp"
#include "pposix/result.hpp"
#include "pposix/unique_fd.hpp"
#include "pposix/util.hpp"

namespace pposix {

enum class socket_domain : unsigned {
  // POSIX-defined socket domains
  inet = AF_INET,
  inet6 = AF_INET6,
  unix_ = AF_UNIX,
  unspecified = AF_UNSPEC,

// Linux-specific socket domains
#if PPOSIX_LINUX_EXTENSION_ENABLED
  local = AF_LOCAL,
  file = AF_FILE,
  ax25 = AF_AX25,
  ipx = AF_IPX,
  appletalk = AF_APPLETALK,
  x25 = AF_X25,
  decnet = AF_DECnet,
  key = AF_KEY,
  netlink = AF_NETLINK,
  packet = AF_PACKET,
  rds = AF_RDS,
  pppox = AF_PPPOX,
  llc = AF_LLC,
  ib = AF_IB,
  mpls = AF_MPLS,
  can = AF_CAN,
  tipc = AF_TIPC,
  bluetooth = AF_BLUETOOTH,
  alg = AF_ALG,
  vsock = AF_VSOCK,
  kcm = AF_KCM,
  xdp = AF_XDP,
#endif
};

enum class socket_type : int {
  stream = SOCK_STREAM,
  dgram = SOCK_DGRAM,
  seqpacket = SOCK_SEQPACKET,
  raw = SOCK_RAW,
};

enum class socket_protocol : int {
  ip = IPPROTO_IP,
  icmp = IPPROTO_ICMP,
  igmp = IPPROTO_IGMP,
  tcp = IPPROTO_TCP,
  pup = IPPROTO_PUP,
  udp = IPPROTO_UDP,
  idp = IPPROTO_IDP,
  raw = IPPROTO_RAW,
};

enum class socket_option : int {
  acceptconn = SO_ACCEPTCONN,
  broadcast = SO_BROADCAST,
  debug = SO_DEBUG,
  dontroute = SO_DONTROUTE,
  error = SO_ERROR,
  keepalive = SO_KEEPALIVE,
  linger = SO_LINGER,
  oobinline = SO_OOBINLINE,
  rcvbuf = SO_RCVBUF,
  rcvlowat = SO_RCVLOWAT,
  rcvtimeo = SO_RCVTIMEO,
  reuseaddr = SO_REUSEADDR,
  sndbuf = SO_SNDBUF,
  sndlowat = SO_SNDLOWAT,
  sndtimeo = SO_SNDTIMEO,
  type = SO_TYPE
};

enum class socket_flag : unsigned {
  none = 0u,

#if PPOSIX_LINUX_EXTENSION_ENABLED
  closexec = SOCK_CLOEXEC,
  nonblock = SOCK_NONBLOCK,
#endif
};

constexpr socket_flag operator|(socket_flag lhs, socket_flag rhs) noexcept {
  return socket_flag{underlying_v(lhs) | underlying_v(rhs)};
}

enum class socket_level : int {
  socket = SOL_SOCKET,
  ip = IPPROTO_IP,
  ipv6 = IPPROTO_IPV6,
  icmp = IPPROTO_ICMP,
  raw = IPPROTO_RAW,
  tcp = IPPROTO_TCP,
  udp = IPPROTO_UDP,
};

using unique_socket_fd = unique_fd;

// Socket options
enum class socket_debug : bool { off = false, on = true };
enum class socket_broadcast : bool { off = false, on = true };
enum class socket_reuseaddr : bool { off = false, on = true };
enum class socket_keepalive : bool { off = false, on = true };

class socket_linger {
 public:
  constexpr socket_linger() noexcept = default;
  constexpr socket_linger(::linger l) noexcept : linger_{l} {}  // NOLINT implicit constructor
  constexpr explicit socket_linger(pposix::seconds duration) noexcept
      : linger_{true, duration.count()} {}
  constexpr socket_linger(bool enabled, pposix::seconds duration) noexcept
      : linger_{enabled, duration.count()} {}

  constexpr socket_linger(const socket_linger &) noexcept = default;
  constexpr socket_linger(socket_linger &&) noexcept = default;

  constexpr socket_linger &operator=(const socket_linger &) noexcept = default;
  constexpr socket_linger &operator=(socket_linger &&) noexcept = default;

  constexpr const ::linger &get() const noexcept { return linger_; }

  constexpr bool enabled() const noexcept { return linger_.l_onoff == 1; }
  constexpr pposix::seconds duration() const noexcept { return pposix::seconds{linger_.l_linger}; }

 private:
  ::linger linger_{};
};

enum class socket_oobinline : bool { off = false, on = true };
enum class socket_sndbuf : int {};
enum class socket_rcvbuf : int {};
enum class socket_dontroute : bool { off = false, on = true };
enum class socket_rcvlowat : int {};

class socket_rcvtimeo {
 public:
  constexpr socket_rcvtimeo() noexcept = default;

  constexpr explicit socket_rcvtimeo(pposix::seconds s) noexcept : timeout_{s.count(), 0u} {}
  constexpr socket_rcvtimeo(pposix::seconds s, pposix::microseconds us) noexcept
      : timeout_{s.count(), us.count()} {}

  constexpr socket_rcvtimeo(const socket_rcvtimeo &) noexcept = default;
  constexpr socket_rcvtimeo(socket_rcvtimeo &&) noexcept = default;

  constexpr socket_rcvtimeo &operator=(const socket_rcvtimeo &) noexcept = default;
  constexpr socket_rcvtimeo &operator=(socket_rcvtimeo &&) noexcept = default;

  constexpr const ::timeval &get() const noexcept { return timeout_; }

  constexpr pposix::seconds seconds() const noexcept { return pposix::seconds{timeout_.tv_sec}; }

  constexpr pposix::microseconds microseconds() const noexcept {
    return pposix::microseconds{timeout_.tv_usec};
  }

 private:
  ::timeval timeout_{};
};

enum class socket_sndlowat : int {};

class socket_sndtimeo {
 public:
  constexpr socket_sndtimeo() noexcept = default;

  constexpr explicit socket_sndtimeo(pposix::seconds s) noexcept : timeout_{s.count(), 0u} {}
  constexpr socket_sndtimeo(pposix::seconds s, pposix::microseconds us) noexcept
      : timeout_{s.count(), us.count()} {}

  constexpr socket_sndtimeo(const socket_sndtimeo &) noexcept = default;
  constexpr socket_sndtimeo(socket_sndtimeo &&) noexcept = default;

  constexpr socket_sndtimeo &operator=(const socket_sndtimeo &) noexcept = default;
  constexpr socket_sndtimeo &operator=(socket_sndtimeo &&) noexcept = default;

  constexpr const ::timeval &get() const noexcept { return timeout_; }

  constexpr pposix::seconds seconds() const noexcept { return pposix::seconds{timeout_.tv_sec}; }

  constexpr pposix::microseconds microseconds() const noexcept {
    return pposix::microseconds{timeout_.tv_usec};
  }

 private:
  ::timeval timeout_{};
};

// Get socket option
enum class socket_acceptconn : bool {};
enum class socket_error : int {};

// Set socket option
class socket {
 public:
  socket() noexcept = default;

  explicit socket(raw_fd fd) noexcept;

  socket(const socket &) = delete;
  socket(socket &&) = default;

  socket &operator=(const socket &) = delete;
  socket &operator=(socket &&) = default;

  static pposix::result<socket> unsafe_make(socket_domain dom, socket_type typ, socket_flag flags,
                                            socket_protocol prot) noexcept;

  std::error_code unsafe_setsockopt(socket_level, socket_option, any_cview) noexcept;

  std::error_code setsockopt(socket_level, socket_debug) noexcept;
  std::error_code setsockopt(socket_level, socket_broadcast) noexcept;
  std::error_code setsockopt(socket_level, socket_reuseaddr) noexcept;
  std::error_code setsockopt(socket_level, socket_keepalive) noexcept;
  std::error_code setsockopt(socket_level, socket_linger) noexcept;
  std::error_code setsockopt(socket_level, socket_oobinline) noexcept;
  std::error_code setsockopt(socket_level, socket_sndbuf) noexcept;
  std::error_code setsockopt(socket_level, socket_rcvbuf) noexcept;
  std::error_code setsockopt(socket_level, socket_dontroute) noexcept;
  std::error_code setsockopt(socket_level, socket_rcvlowat) noexcept;
  std::error_code setsockopt(socket_level, socket_rcvtimeo) noexcept;
  std::error_code setsockopt(socket_level, socket_sndlowat) noexcept;
  std::error_code setsockopt(socket_level, socket_sndtimeo) noexcept;

  result<socklen_t> unsafe_getsockopt(socket_level, socket_option, any_view) noexcept;

  template <class T>
  result<T> getsockopt(const socket_level l) noexcept {
    if constexpr (std::is_same_v<T, socket_debug>) {
      return getsockopt_bool<socket_debug>(l, socket_option::debug);
    } else if (std::is_same_v<T, socket_broadcast>) {
      return getsockopt_bool<socket_broadcast>(l, socket_option::broadcast);
    } else if (std::is_same_v<T, socket_reuseaddr>) {
      return getsockopt_bool<socket_reuseaddr>(l, socket_option::reuseaddr);
    } else if (std::is_same_v<T, socket_acceptconn>) {
      return getsockopt_bool<socket_acceptconn>(l, socket_option::acceptconn);
    } else if (std::is_same_v<T, socket_keepalive>) {
      return getsockopt_bool<socket_keepalive>(l, socket_option::keepalive);
    } else if (std::is_same_v<T, socket_linger>) {
      return getsockopt_struct<socket_linger>(l, socket_option::linger);
    } else if (std::is_same_v<T, socket_oobinline>) {
      return getsockopt_bool<socket_oobinline>(l, socket_option::oobinline);
    } else if (std::is_same_v<T, socket_sndbuf>) {
      return getsockopt_int<socket_sndbuf>(l, socket_option::sndbuf);
    } else if (std::is_same_v<T, socket_rcvbuf>) {
      return getsockopt_int<socket_rcvbuf>(l, socket_option::rcvbuf);
    } else if (std::is_same_v<T, socket_error>) {
      return getsockopt_int<socket_error>(l, socket_option::error);
    } else if (std::is_same_v<T, socket_type>) {
      return getsockopt_int<socket_type>(l, socket_option::type);
    } else if (std::is_same_v<T, socket_dontroute>) {
      return getsockopt_bool<socket_dontroute>(l, socket_option::dontroute);
    } else if (std::is_same_v<T, socket_rcvlowat>) {
      return getsockopt_int<socket_rcvlowat>(l, socket_option::rcvlowat);
    } else if (std::is_same_v<T, socket_rcvtimeo>) {
      return getsockopt_struct<socket_rcvtimeo>(l, socket_option::rcvtimeo);
    } else if (std::is_same_v<T, socket_sndlowat>) {
      return getsockopt_int<socket_sndlowat>(l, socket_option::sndlowat);
    } else if (std::is_same_v<T, socket_sndtimeo>) {
      return getsockopt_struct<socket_sndtimeo>(l, socket_option::sndtimeo);
    } else {
      static_assert(always_false<T>, "Unsupported socket option type.");
    }
  }

 private:
  std::error_code setsockopt_int(socket_level l, socket_option o, int i) noexcept;
  std::error_code setsockopt_bool(socket_level l, socket_option o, bool b) noexcept;

  template <class Result>
  result<Result> getsockopt_int(socket_level l, socket_option o) noexcept {
    int val{};

    const auto result = unsafe_getsockopt(l, o, any_view{&val});
    return result_map<Result>(result, [&](int /*ignored*/) { return Result{val}; });
  }

  template <class Result>
  result<bool> getsockopt_bool(socket_level l, socket_option o) noexcept {
    int val{};

    const auto result = unsafe_getsockopt(l, o, any_view{&val});
    return result_map<Result>(result, [&](int /*ignored*/) { return Result{val != 0}; });
  }

  template <class Result>
  result<Result> getsockopt_struct(socket_level l, socket_option o) noexcept {
    static_assert(std::is_class_v<Result>);

    Result val{};

    const auto result = unsafe_getsockopt(l, o, any_view{&val});
    return result_map<Result>(result, [&](int /*ignored*/) { return val; });
  }

 private:
  unique_socket_fd socket_fd_{};
};
}