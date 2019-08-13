#include "pposix_test.hpp"

#include <algorithm>
#include <cstddef>

#include "pposix/byte_span.hpp"

void require_is_empty_buffer_span(const pposix::byte_span &buffer) {
  require(buffer.empty());
  require(buffer.length() == 0u);
  require(buffer.cbegin() == buffer.cend());
  require(std::distance(buffer.cbegin(), buffer.cend()) == 0u);
}

scenario("Buffer spans can be default constructed") {
  pposix::byte_span buffer{};

  require_is_empty_buffer_span(buffer);
}

scenario("Buffer spans can be constructed with existing byte buffers") {
  constexpr std::size_t RAW_BUFFER_SIZE = 10u;

  given("a byte c-array") {
    std::byte raw_buffer[RAW_BUFFER_SIZE];

    pposix::byte_span buffer{raw_buffer};

    require(not buffer.empty());
    require(buffer.length() == RAW_BUFFER_SIZE);
    require(buffer.data() == raw_buffer);
    require(std::distance(buffer.begin(), buffer.end()) == RAW_BUFFER_SIZE);
    require(std::distance(buffer.cbegin(), buffer.cend()) == RAW_BUFFER_SIZE);
  }

  given("a byte array") {
    std::array<std::byte, RAW_BUFFER_SIZE> raw_buffer{};

    pposix::byte_span buffer{raw_buffer};

    require(not buffer.empty());
    require(buffer.length() == RAW_BUFFER_SIZE);
    require(buffer.length() == raw_buffer.size());
    require(buffer.data() == raw_buffer.data());
    require(std::distance(buffer.begin(), buffer.end()) == RAW_BUFFER_SIZE);
    require(std::distance(buffer.cbegin(), buffer.cend()) == RAW_BUFFER_SIZE);
  }

  given("a pointer and size") {
    std::byte raw_buffer[RAW_BUFFER_SIZE];

    std::byte *raw_ptr = raw_buffer;

    pposix::byte_span buffer{raw_ptr, 1u};

    require(not buffer.empty());
    require(buffer.length() == 1u);
    require(buffer.data() == raw_ptr);
    require(std::distance(buffer.begin(), buffer.end()) == 1u);
    require(std::distance(buffer.cbegin(), buffer.cend()) == 1u);
  }

  given("a pointer and zero size") {
    std::byte raw_buffer[RAW_BUFFER_SIZE];

    std::byte *raw_ptr = raw_buffer;

    pposix::byte_span buffer{raw_ptr, 0u};

    require(buffer.empty());
    require(buffer.length() == 0u);
    require(buffer.data() == raw_ptr);
    require(std::distance(buffer.begin(), buffer.end()) == 0u);
    require(std::distance(buffer.cbegin(), buffer.cend()) == 0u);
  }

  given("a null pointer and size") {
    pposix::byte_span buffer{nullptr, 10u};

    require_is_empty_buffer_span(buffer);
  }

  given("a null pointer and zero size") {
    pposix::byte_span buffer{nullptr, 0u};

    require_is_empty_buffer_span(buffer);
  }
}

scenario("Buffer spans can be sliced") {
  given("empty buffer span") {
    pposix::byte_span buffer{};

    require(buffer.empty());

    when("sliced by no offset and no count") {
      const auto slice = buffer.subspan(0u, 0u);

      then("the slice is empty") { require_is_empty_buffer_span(slice); }
    }

    when("sliced by the max offset and no count") {
      const auto slice = buffer.subspan(std::numeric_limits<std::size_t>::max(), 0u);

      then("the slice is empty") { require_is_empty_buffer_span(slice); }
    }

    when("sliced by no offset and max count") {
      const auto slice = buffer.subspan(0u, std::numeric_limits<std::size_t>::max());

      then("the slice is empty") { require_is_empty_buffer_span(slice); }
    }
    when("sliced by the max possible offset and max count") {
      const auto slice = buffer.subspan(std::numeric_limits<std::size_t>::max(),
                                        std::numeric_limits<std::size_t>::max());

      then("the slice is empty") { require_is_empty_buffer_span(slice); }
    }
  }

  given("a non-empty buffer span") {
    std::byte raw_buffer[10u]{};
    pposix::byte_span buffer{raw_buffer};

    when("slicing the entire string") {
      const auto slice = buffer.subspan(0u, buffer.length());

      require(not slice.empty());
      require(slice.length() == buffer.length());
      require(std::equal(slice.cbegin(), slice.cend(), buffer.cbegin()));
    }

    when("slice offset is equal to buffer length and zero count") {
      const auto slice = buffer.subspan(buffer.length(), 0u);

      then("slice is empty") { require_is_empty_buffer_span(slice); }
    }

    when("slice offset is zero and count is zero") {
      const auto slice = buffer.subspan(0u, 0u);

      then("slice is empty") { require_is_empty_buffer_span(slice); }
    }
  }
}
