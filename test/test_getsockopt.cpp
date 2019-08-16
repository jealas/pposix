#include "pposix_test.hpp"

#include <utility>

#include "pposix/socket.hpp"
#include "pposix/unique_fd.hpp"

using namespace pposix;

scenario("Can get default socket options") {
  socket_type type;
  const std::initializer_list<socket_type> socket_types{socket_type::seqpacket,
                                                        socket_type::stream, socket_type::dgram};

  generate(type, socket_types);

  given("a valid unix/local socket") {
    auto socket_result{socket(socket_domain::unix_, type, socket_flag::none, socket_protocol{0})};

    require(socket_result);

    pposix::unique_fd<pposix::socket_fd> sockfd{std::move(*socket_result)};

    require(sockfd);
    require(not sockfd.empty());
    require(sockfd.raw() != pposix::nullfd);

    when("getting the default debug option") {
      const auto option = getsockopt<debug>(*sockfd, socket_level::socket);

      require(option.has_value());
      require(not option.has_error());

      then("the value is off") { require(option.value() == debug::off); }
    }

    when("getting the default broadcast option") {
      const auto option = getsockopt<broadcast>(*sockfd, socket_level::socket);

      require(option.has_value());
      require(not option.has_error());

      then("the value is off") { require(option.value() == broadcast::off); }
    }

    when("getting the default don't route option") {
      const auto option = getsockopt<dontroute>(*sockfd, socket_level::socket);

      require(option.has_value());
      require(not option.has_error());

      then("the value is off") { require(option.value() == dontroute::off); }
    }

    when("getting the default keep alive option") {
      const auto option = getsockopt<keepalive>(*sockfd, socket_level::socket);

      require(option.has_value());
      require(not option.has_error());

      then("the value is off") { require(option.value() == keepalive::off); }
    }

    when("getting the default linger option") {
      const auto option = getsockopt<pposix::linger>(*sockfd, socket_level::socket);

      require(not option.has_error());

      then("the value is 0") {
        const auto &value = option.value();

        require(not value.enabled());
        require(value.duration() == pposix::seconds{0});
      }
    }

    when("getting the default out of band inline option") {
      const auto option = getsockopt<oobinline>(*sockfd, socket_level::socket);

      require(not option.has_error());

      then("the value is off") {
        const auto &value = option.value();

        require(value == oobinline::off);
      }
    }

    when("getting the default receive buffer size") {
      const auto option = getsockopt<rcvbuf>(*sockfd, socket_level::socket);

      then("the buffer size is returned") {
        require(option.has_value());
        require(not option.has_error());

        // Cannot check the exact value returned because it is implementation defined
      }
    }

    when("getting the default receive low water line value") {
      const auto option = getsockopt<rcvlowat>(*sockfd, socket_level::socket);

      then("the default value 1 is returned") {
        const auto &value = option.value();

        require(not option.has_error());

        require(value == rcvlowat{1});
      }
    }

    when("getting the default receive timeout value") {
      const auto option = getsockopt<rcvtimeo>(*sockfd, socket_level::socket);

      then("the default value is 0 seconds and 0 microseconds") {
        const auto &value = option.value();

        require(not option.has_error());

        require(value.seconds() == pposix::seconds{0});
        require(value.microseconds() == pposix::seconds{0});
      }
    }

    when("getting the default reuse address value") {
      const auto option = getsockopt<reuseaddr>(*sockfd, socket_level::socket);

      then("the default value is off") {
        const auto &value = option.value();

        require(not option.has_error());

        require(value == reuseaddr::off);
      }
    }

    when("getting the default send buffer size value") {
      const auto option = getsockopt<sndbuf>(*sockfd, socket_level::socket);

      then("the buffer size is returned") {
        require(option.has_value());
        require(not option.has_error());

        // Cannot check the exact value returned because it is implementation defined
      }
    }

    when("getting the send low water line value works") {
      const auto option = getsockopt<sndlowat>(*sockfd, socket_level::socket);

      require(not option.has_error());
    }

    when("getting the default send timeout value") {
      const auto option = getsockopt<sndtimeo>(*sockfd, socket_level::socket);

      then("the default value is 0 seconds and 0 microseconds") {
        const auto &value = option.value();

        require(not option.has_error());

        require(value.seconds() == pposix::seconds{0});
        require(value.microseconds() == pposix::seconds{0});
      }
    }

    when("getting the type value") {
      const auto option = getsockopt<socket_type>(*sockfd, socket_level::socket);

      require(not option.has_error());

      then("the value is the same as the type used to construct the socket") {
        const auto &value = option.value();

        require(value == type);
      }
    }
  }
}