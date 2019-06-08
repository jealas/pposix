#include <catch2/catch.hpp>

#include "pposix/capi/socket/socket.hpp"
#include "pposix/capi/socket/sockopt.hpp"
#include "pposix/unique_fd.hpp"

namespace sock = pposix::capi::socket;

SCENARIO("Can get default socket options", "[pposix][capi][socket]") {
  GIVEN("a valid unix/local socket") {
    const auto domain = GENERATE(sock::domain::local, sock::domain::unix_);
    const auto type = GENERATE(sock::type::seqpacket, sock::type::stream, sock::type::dgram);

    const pposix::unique_fd<sock::socketfd> sockfd{
        sock::socket(domain, type, sock::flag::none, sock::protocol{0})};

    REQUIRE(sockfd);
    REQUIRE(not sockfd.empty());
    REQUIRE(sockfd.raw() != pposix::capi::nullfd);

    WHEN("getting the default debug option") {
      const auto option = sock::getsockopt<sock::debug>(*sockfd, sock::level::socket);

      REQUIRE(option.value());
      REQUIRE(not option.error());

      THEN("the value is off") { REQUIRE(*option.value() == sock::debug::off); }
    }

    WHEN("getting the default broadcast option") {
      const auto option = sock::getsockopt<sock::broadcast>(*sockfd, sock::level::socket);

      REQUIRE(option.value());
      REQUIRE(not option.error());

      THEN("the value is off") { REQUIRE(*option.value() == sock::broadcast::off); }
    }

    WHEN("getting the default don't route option") {
      const auto option = sock::getsockopt<sock::dontroute>(*sockfd, sock::level::socket);

      REQUIRE(option.value());
      REQUIRE(not option.error());

      THEN("the value is off") { REQUIRE(*option.value() == sock::dontroute::off); }
    }

    WHEN("getting the default keep alive option") {
      const auto option = sock::getsockopt<sock::keepalive>(*sockfd, sock::level::socket);

      REQUIRE(option.value());
      REQUIRE(not option.error());

      THEN("the value is off") { REQUIRE(*option.value() == sock::keepalive::off); }
    }

    WHEN("getting the default linger option") {
      const auto option = sock::getsockopt<sock::linger>(*sockfd, sock::level::socket);

      REQUIRE(not option.error());

      THEN("the value is 0") {
        auto const* const value = option.value();

        REQUIRE(value);
        REQUIRE(not value->enabled());
        REQUIRE(value->duration() == pposix::seconds{0});
      }
    }

    WHEN("getting the default out of band inline option") {
      const auto option = sock::getsockopt<sock::oobinline>(*sockfd, sock::level::socket);

      REQUIRE(not option.error());

      THEN("the value is off") {
        auto const* const value = option.value();

        REQUIRE(value);
        REQUIRE(*value == sock::oobinline::off);
      }
    }

    WHEN("getting the default receive buffer size") {
      const auto option = sock::getsockopt<sock::rcvbuf>(*sockfd, sock::level::socket);

      THEN("the buffer size is returned") {
        REQUIRE(option.value());
        REQUIRE(not option.error());

        // Cannot check the exact value returned because it is implementation defined
      }
    }

    WHEN("getting the default receive low water line value") {
      const auto option = sock::getsockopt<sock::rcvlowat>(*sockfd, sock::level::socket);

      THEN("the default value 1 is returned") {
        auto const* const value = option.value();

        REQUIRE(value);
        REQUIRE(not option.error());

        REQUIRE(*value == sock::rcvlowat{1});
      }
    }

    WHEN("getting the default receive timeout value") {
      const auto option = sock::getsockopt<sock::rcvtimeo>(*sockfd, sock::level::socket);

      THEN("the default value is 0 seconds and 0 microseconds") {
        auto const* const value = option.value();

        REQUIRE(value);
        REQUIRE(not option.error());

        REQUIRE(value->seconds() == pposix::seconds{0});
        REQUIRE(value->microseconds() == pposix::seconds{0});
      }
    }

    WHEN("getting the default reuse address value") {
      const auto option = sock::getsockopt<sock::reuseaddr>(*sockfd, sock::level::socket);

      THEN("the default value is off") {
        auto const* const value = option.value();

        REQUIRE(value);
        REQUIRE(not option.error());

        REQUIRE(*value == sock::reuseaddr::off);
      }
    }

    WHEN("getting the default send buffer size value") {
      const auto option = sock::getsockopt<sock::sndbuf>(*sockfd, sock::level::socket);

      THEN("the buffer size is returned") {
        REQUIRE(option.value());
        REQUIRE(not option.error());

        // Cannot check the exact value returned because it is implementation defined
      }
    }

    WHEN("getting the send low water line value") {
      const auto option = sock::getsockopt<sock::sndlowat>(*sockfd, sock::level::socket);

      THEN("the default value 1 is returned") {
        auto const* const value = option.value();

        REQUIRE(value);
        REQUIRE(not option.error());

        REQUIRE(*value == sock::sndlowat{1});
      }
    }

    WHEN("getting the default send timeout value") {
      const auto option = sock::getsockopt<sock::sndtimeo>(*sockfd, sock::level::socket);

      THEN("the default value is 0 seconds and 0 microseconds") {
        auto const* const value = option.value();

        REQUIRE(value);
        REQUIRE(not option.error());

        REQUIRE(value->seconds() == pposix::seconds{0});
        REQUIRE(value->microseconds() == pposix::seconds{0});
      }
    }

    WHEN("getting the type value") {
      const auto option = sock::getsockopt<sock::type>(*sockfd, sock::level::socket);

      REQUIRE(not option.error());

      THEN("the value is the same as the type used to construct the socket") {
        auto const* const value = option.value();

        REQUIRE(value);
        REQUIRE(*value == type);
      }
    }
  }
}