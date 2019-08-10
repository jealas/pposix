#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "pposix/any_view.hpp"
#include "pposix/duration.hpp"
#include "pposix/extension.hpp"
#include "pposix/fd.hpp"
#include "pposix/result.hpp"
#include "pposix/unique_fd.hpp"
#include "pposix/util.hpp"

namespace pposix {

// Socket file descriptor
struct socket_fd_tag {};
using socket_fd = fd<socket_fd_tag>;

// Socket domain
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

// Socket type
enum class socket_type : int {
  stream = SOCK_STREAM,
  dgram = SOCK_DGRAM,
  seqpacket = SOCK_SEQPACKET,
  raw = SOCK_RAW,
};

// Socket protocol
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

// Socket option
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

// Socket flag
enum class socket_flag : unsigned {
  none = 0u,

#if PPOSIX_LINUX_EXTENSION_ENABLED
  closexec = SOCK_CLOEXEC,
  nonblock = SOCK_NONBLOCK,
#endif
};

constexpr socket_flag operator|(socket_flag lhs, socket_flag rhs) noexcept {
  return socket_flag{underlying_value(lhs) | underlying_value(rhs)};
}

// Socket level
enum class socket_level : int {
  socket = SOL_SOCKET,
  ip = IPPROTO_IP,
  ipv6 = IPPROTO_IPV6,
  icmp = IPPROTO_ICMP,
  raw = IPPROTO_RAW,
  tcp = IPPROTO_TCP,
  udp = IPPROTO_UDP,
};

// Socket
result<unique_fd<socket_fd>> socket(socket_domain dom, socket_type typ, socket_flag flags,
                                    socket_protocol prot) noexcept;

// Socket options
enum class debug : bool { off = false, on = true };
enum class broadcast : bool { off = false, on = true };
enum class reuseaddr : bool { off = false, on = true };
enum class keepalive : bool { off = false, on = true };

class linger {
 public:
  constexpr linger() noexcept = default;
  constexpr linger(::linger l) noexcept : linger_{l} {}  // NOLINT implicit constructor
  constexpr explicit linger(pposix::seconds duration) noexcept : linger_{true, duration.count()} {}
  constexpr linger(bool enabled, pposix::seconds duration) noexcept
      : linger_{enabled, duration.count()} {}

  constexpr linger(const linger &) noexcept = default;
  constexpr linger(linger &&) noexcept = default;

  constexpr linger &operator=(const linger &) noexcept = default;
  constexpr linger &operator=(linger &&) noexcept = default;

  constexpr const ::linger &get() const noexcept { return linger_; }

  constexpr bool enabled() const noexcept { return linger_.l_onoff; }
  constexpr pposix::seconds duration() const noexcept { return pposix::seconds{linger_.l_linger}; }

 private:
  ::linger linger_{};
};

enum class oobinline : bool { off = false, on = true };
enum class sndbuf : int {};
enum class rcvbuf : int {};
enum class dontroute : bool { off = false, on = true };
enum class rcvlowat : int {};

class rcvtimeo {
 public:
  constexpr rcvtimeo() noexcept = default;

  constexpr explicit rcvtimeo(pposix::seconds s) noexcept : timeout_{s.count(), 0u} {}
  constexpr rcvtimeo(pposix::seconds s, pposix::microseconds us) noexcept
      : timeout_{s.count(), us.count()} {}

  constexpr rcvtimeo(const rcvtimeo &) noexcept = default;
  constexpr rcvtimeo(rcvtimeo &&) noexcept = default;

  constexpr rcvtimeo &operator=(const rcvtimeo &) noexcept = default;
  constexpr rcvtimeo &operator=(rcvtimeo &&) noexcept = default;

  constexpr const ::timeval &get() const noexcept { return timeout_; }

  constexpr pposix::seconds seconds() const noexcept { return pposix::seconds{timeout_.tv_sec}; }

  constexpr pposix::microseconds microseconds() const noexcept {
    return pposix::microseconds{timeout_.tv_usec};
  }

 private:
  ::timeval timeout_{};
};

enum class sndlowat : int {};

class sndtimeo {
 public:
  constexpr sndtimeo() noexcept = default;

  constexpr explicit sndtimeo(pposix::seconds s) noexcept : timeout_{s.count(), 0u} {}
  constexpr sndtimeo(pposix::seconds s, pposix::microseconds us) noexcept
      : timeout_{s.count(), us.count()} {}

  constexpr sndtimeo(const sndtimeo &) noexcept = default;
  constexpr sndtimeo(sndtimeo &&) noexcept = default;

  constexpr sndtimeo &operator=(const sndtimeo &) noexcept = default;
  constexpr sndtimeo &operator=(sndtimeo &&) noexcept = default;

  constexpr const ::timeval &get() const noexcept { return timeout_; }

  constexpr pposix::seconds seconds() const noexcept { return pposix::seconds{timeout_.tv_sec}; }

  constexpr pposix::microseconds microseconds() const noexcept {
    return pposix::microseconds{timeout_.tv_usec};
  }

 private:
  ::timeval timeout_{};
};

// Set socket option
std::error_code setsockopt(socket_fd, socket_level, socket_option, any_cview) noexcept;
std::error_code setsockopt(socket_fd, socket_level, debug) noexcept;
std::error_code setsockopt(socket_fd, socket_level, broadcast) noexcept;
std::error_code setsockopt(socket_fd, socket_level, reuseaddr) noexcept;
std::error_code setsockopt(socket_fd, socket_level, keepalive) noexcept;
std::error_code setsockopt(socket_fd, socket_level, linger) noexcept;
std::error_code setsockopt(socket_fd, socket_level, oobinline) noexcept;
std::error_code setsockopt(socket_fd, socket_level, sndbuf) noexcept;
std::error_code setsockopt(socket_fd, socket_level, rcvbuf) noexcept;
std::error_code setsockopt(socket_fd, socket_level, dontroute) noexcept;
std::error_code setsockopt(socket_fd, socket_level, rcvlowat) noexcept;
std::error_code setsockopt(socket_fd, socket_level, rcvtimeo) noexcept;
std::error_code setsockopt(socket_fd, socket_level, sndlowat) noexcept;
std::error_code setsockopt(socket_fd, socket_level, sndtimeo) noexcept;

// Get socket option
enum class acceptconn : bool {};
enum class error : int {};

result<socklen_t> getsockopt(socket_fd, socket_level, socket_option, any_view) noexcept;

template <class T>
result<T> getsockopt(socket_fd, socket_level) noexcept;

}