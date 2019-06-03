#include <catch2/catch.hpp>

#include <algorithm>
#include <cstddef>

#include "pposix/buffer_span.hpp"

void require_is_empty_buffer_span(const pposix::buffer_span &buffer) {
  REQUIRE(buffer.empty());
  REQUIRE(buffer.length() == 0u);
  REQUIRE(buffer.cbegin() == buffer.cend());
  REQUIRE(std::distance(buffer.cbegin(), buffer.cend()) == 0u);
}

SCENARIO("Buffer spans can be default constructed", "[pposix][buffer_span]") {
  pposix::buffer_span buffer{};

  require_is_empty_buffer_span(buffer);
}

SCENARIO("Buffer spans can be constructed with existing byte buffers", "[pposix][buffer_span]") {
  constexpr std::size_t RAW_BUFFER_SIZE = 10u;

  GIVEN("a byte c-array") {
    std::byte raw_buffer[RAW_BUFFER_SIZE];

    pposix::buffer_span buffer{raw_buffer};

    REQUIRE(not buffer.empty());
    REQUIRE(buffer.length() == RAW_BUFFER_SIZE);
    REQUIRE(buffer.data() == raw_buffer);
    REQUIRE(std::distance(buffer.begin(), buffer.end()) == RAW_BUFFER_SIZE);
    REQUIRE(std::distance(buffer.cbegin(), buffer.cend()) == RAW_BUFFER_SIZE);
  }

  GIVEN("a byte array") {
    std::array<std::byte, RAW_BUFFER_SIZE> raw_buffer{};

    pposix::buffer_span buffer{raw_buffer};

    REQUIRE(not buffer.empty());
    REQUIRE(buffer.length() == RAW_BUFFER_SIZE);
    REQUIRE(buffer.length() == raw_buffer.size());
    REQUIRE(buffer.data() == raw_buffer.data());
    REQUIRE(std::distance(buffer.begin(), buffer.end()) == RAW_BUFFER_SIZE);
    REQUIRE(std::distance(buffer.cbegin(), buffer.cend()) == RAW_BUFFER_SIZE);
  }

  GIVEN("a pointer and size") {
    std::byte raw_buffer[RAW_BUFFER_SIZE];

    std::byte *raw_ptr = raw_buffer;

    pposix::buffer_span buffer{raw_ptr, 1u};

    REQUIRE(not buffer.empty());
    REQUIRE(buffer.length() == 1u);
    REQUIRE(buffer.data() == raw_ptr);
    REQUIRE(std::distance(buffer.begin(), buffer.end()) == 1u);
    REQUIRE(std::distance(buffer.cbegin(), buffer.cend()) == 1u);
  }

  GIVEN("a pointer and zero size") {
    std::byte raw_buffer[RAW_BUFFER_SIZE];

    std::byte *raw_ptr = raw_buffer;

    pposix::buffer_span buffer{raw_ptr, 0u};

    REQUIRE(buffer.empty());
    REQUIRE(buffer.length() == 0u);
    REQUIRE(buffer.data() == raw_ptr);
    REQUIRE(std::distance(buffer.begin(), buffer.end()) == 0u);
    REQUIRE(std::distance(buffer.cbegin(), buffer.cend()) == 0u);
  }

  GIVEN("a null pointer and size") {
    pposix::buffer_span buffer{nullptr, 10u};

    require_is_empty_buffer_span(buffer);
  }

  GIVEN("a null pointer and zero size") {
    pposix::buffer_span buffer{nullptr, 0u};

    require_is_empty_buffer_span(buffer);
  }
}

SCENARIO("Buffer spans can be sliced", "[pposix][buffer_span]") {
  GIVEN("empty buffer span") {
    pposix::buffer_span buffer{};

    REQUIRE(buffer.empty());

    WHEN("sliced by no offset and no count") {
      const auto slice = buffer.subspan(0u, 0u);

      THEN("the slice is empty") { require_is_empty_buffer_span(slice); }
    }

    WHEN("sliced by the max offset and no count") {
      const auto slice = buffer.subspan(std::numeric_limits<std::size_t>::max(), 0u);

      THEN("the slice is empty") { require_is_empty_buffer_span(slice); }
    }

    WHEN("sliced by no offset and max count") {
      const auto slice = buffer.subspan(0u, std::numeric_limits<std::size_t>::max());

      THEN("the slice is empty") { require_is_empty_buffer_span(slice); }
    }
    WHEN("sliced by the max possible offset and max count") {
      const auto slice = buffer.subspan(std::numeric_limits<std::size_t>::max(),
                                        std::numeric_limits<std::size_t>::max());

      THEN("the slice is empty") { require_is_empty_buffer_span(slice); }
    }
  }

  GIVEN("a non-empty buffer span") {
    std::byte raw_buffer[10u];
    pposix::buffer_span buffer{raw_buffer};

    WHEN("slicing the entire string") {
      const auto slice = buffer.subspan(0u, buffer.length());

      REQUIRE(not slice.empty());
      REQUIRE(slice.length() == buffer.length());
      REQUIRE(std::equal(slice.cbegin(), slice.cend(), buffer.cbegin()));
    }

    WHEN("slice offset is equal to buffer length and zero count") {
      const auto slice = buffer.subspan(buffer.length(), 0u);

      THEN("slice is empty") { require_is_empty_buffer_span(slice); }
    }

    WHEN("slice offset is zero and count is zero") {
      const auto slice = buffer.subspan(0u, 0u);

      THEN("slice is empty") { require_is_empty_buffer_span(slice); }
    }
  }
}
