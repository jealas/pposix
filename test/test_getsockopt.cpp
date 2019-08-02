#include <catch2/catch.hpp>

#include <utility>

#include "pposix/socket.hpp"
#include "pposix/unique_fd.hpp"

using namespace pposix;

SCENARIO("Can get default socket options", "[pposix][socket]") {
  GIVEN("a valid unix/local socket") {
    const auto domain = GENERATE(socket_domain::local, socket_domain::unix_);
    const auto type = GENERATE(socket_type::seqpacket, socket_type::stream, socket_type::dgram);

    auto socket_result{socket(domain, type, socket_flag::none, socket_protocol{0})};

    REQUIRE(socket_result);

    pposix::unique_fd<pposix::socket_fd> sockfd{std::move(*socket_result)};

    REQUIRE(sockfd);
    REQUIRE(not sockfd.empty());
    REQUIRE(sockfd.raw() != pposix::nullfd);

    WHEN("getting the default debug option") {
      const auto option = getsockopt<debug>(*sockfd, socket_level::socket);

      REQUIRE(option.has_value());
      REQUIRE(not option.has_error());

      THEN("the value is off") { REQUIRE(option.value() == debug::off); }
    }

    WHEN("getting the default broadcast option") {
      const auto option = getsockopt<broadcast>(*sockfd, socket_level::socket);

      REQUIRE(option.has_value());
      REQUIRE(not option.has_error());

      THEN("the value is off") { REQUIRE(option.value() == broadcast::off); }
    }

    WHEN("getting the default don't route option") {
      const auto option = getsockopt<dontroute>(*sockfd, socket_level::socket);

      REQUIRE(option.has_value());
      REQUIRE(not option.has_error());

      THEN("the value is off") { REQUIRE(option.value() == dontroute::off); }
    }

    WHEN("getting the default keep alive option") {
      const auto option = getsockopt<keepalive>(*sockfd, socket_level::socket);

      REQUIRE(option.has_value());
      REQUIRE(not option.has_error());

      THEN("the value is off") { REQUIRE(option.value() == keepalive::off); }
    }

    WHEN("getting the default linger option") {
      const auto option = getsockopt<pposix::linger>(*sockfd, socket_level::socket);

      REQUIRE(not option.has_error());

      THEN("the value is 0") {
        const auto &value = option.value();

        REQUIRE(not value.enabled());
        REQUIRE(value.duration() == pposix::seconds{0});
      }
    }

    WHEN("getting the default out of band inline option") {
      const auto option = getsockopt<oobinline>(*sockfd, socket_level::socket);

      REQUIRE(not option.has_error());

      THEN("the value is off") {
        const auto &value = option.value();

        REQUIRE(value == oobinline::off);
      }
    }

    WHEN("getting the default receive buffer size") {
      const auto option = getsockopt<rcvbuf>(*sockfd, socket_level::socket);

      THEN("the buffer size is returned") {
        REQUIRE(option.has_value());
        REQUIRE(not option.has_error());

        // Cannot check the exact value returned because it is implementation defined
      }
    }

    WHEN("getting the default receive low water line value") {
      const auto option = getsockopt<rcvlowat>(*sockfd, socket_level::socket);

      THEN("the default value 1 is returned") {
        const auto &value = option.value();

        REQUIRE(not option.has_error());

        REQUIRE(value == rcvlowat{1});
      }
    }

    WHEN("getting the default receive timeout value") {
      const auto option = getsockopt<rcvtimeo>(*sockfd, socket_level::socket);

      THEN("the default value is 0 seconds and 0 microseconds") {
        const auto &value = option.value();

        REQUIRE(not option.has_error());

        REQUIRE(value.seconds() == pposix::seconds{0});
        REQUIRE(value.microseconds() == pposix::seconds{0});
      }
    }

    WHEN("getting the default reuse address value") {
      const auto option = getsockopt<reuseaddr>(*sockfd, socket_level::socket);

      THEN("the default value is off") {
        const auto &value = option.value();

        REQUIRE(not option.has_error());

        REQUIRE(value == reuseaddr::off);
      }
    }

    WHEN("getting the default send buffer size value") {
      const auto option = getsockopt<sndbuf>(*sockfd, socket_level::socket);

      THEN("the buffer size is returned") {
        REQUIRE(option.has_value());
        REQUIRE(not option.has_error());

        // Cannot check the exact value returned because it is implementation defined
      }
    }

    WHEN("getting the send low water line value") {
      const auto option = getsockopt<sndlowat>(*sockfd, socket_level::socket);

      THEN("the default value 1 is returned") {
        const auto &value = option.value();

        REQUIRE(not option.has_error());

        REQUIRE(value == sndlowat{1});
      }
    }

    WHEN("getting the default send timeout value") {
      const auto option = getsockopt<sndtimeo>(*sockfd, socket_level::socket);

      THEN("the default value is 0 seconds and 0 microseconds") {
        const auto &value = option.value();

        REQUIRE(not option.has_error());

        REQUIRE(value.seconds() == pposix::seconds{0});
        REQUIRE(value.microseconds() == pposix::seconds{0});
      }
    }

    WHEN("getting the type value") {
      const auto option = getsockopt<socket_type>(*sockfd, socket_level::socket);

      REQUIRE(not option.has_error());

      THEN("the value is the same as the type used to construct the socket") {
        const auto &value = option.value();

        REQUIRE(value == type);
      }
    }
  }
}