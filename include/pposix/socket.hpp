#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "pposix/any_view.hpp"
#include "pposix/duration.hpp"
#include "pposix/extension.hpp"
#include "pposix/fd.hpp"
#include "pposix/platform.hpp"
#include "pposix/result.hpp"
#include "pposix/util.hpp"

namespace pposix {

namespace capi {

using socket_fd_t = int;

struct raw_socket_fd : raw_fd {
  using raw_fd::raw_fd;
};

using socket_fd =
    descriptor<raw_socket_fd,
               descriptor_constant<raw_socket_fd, socket_fd_t, static_cast<socket_fd_t>(-1)>,
               close_raw_fd>;

enum class socket_domain : int {
  // POSIX-defined socket domains
  inet = AF_INET,
  inet6 = AF_INET6,
  unix_ = AF_UNIX,
  unspecified = AF_UNSPEC,

// Linux-specific socket domains
#if PPOSIX_LINUX_EXTENSION_ENABLED
  local = AF_LOCAL,
  appletalk = AF_APPLETALK,
  decnet = AF_DECnet,

#if !PPOSIX_PLATFORM_CYGWIN
  file = AF_FILE,
  ax25 = AF_AX25,
  ipx = AF_IPX,
  x25 = AF_X25,
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
  type = SO_TYPE,

#if PPOSIX_LINUX_EXTENSION_ENABLED && !PPOSIX_PLATFORM_CYGWIN
  zerocpy = SO_ZEROCOPY
#endif
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

// Socket options
enum class socket_debug : bool { off = false, on = true };
enum class socket_broadcast : bool { off = false, on = true };
enum class socket_reuseaddr : bool { off = false, on = true };
enum class socket_keepalive : bool { off = false, on = true };

using linger_time_t = decltype(::linger::l_linger);

class socket_linger {
 public:
  constexpr socket_linger() noexcept = default;
  constexpr socket_linger(::linger l) noexcept : linger_{l} {}  // NOLINT implicit constructor

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

pposix::result<socket_fd> socket(socket_domain dom, socket_type typ, socket_flag flags,
                                 socket_protocol prot) noexcept;

std::error_code setsockopt(raw_socket_fd fd, socket_level, socket_option, any_cview) noexcept;
result<socklen_t> getsockopt(raw_socket_fd fd, socket_level, socket_option, any_view) noexcept;

}  // namespace capi

// Set socket option
class socket {
 public:
  socket() noexcept = default;

  explicit socket(capi::socket_fd fd) noexcept;

  socket(const socket &) = delete;
  socket(socket &&) = default;

  socket &operator=(const socket &) = delete;
  socket &operator=(socket &&) = default;

 private:
  capi::socket_fd socket_fd_{};
};
}  // namespace pposix