#include <catch2/catch.hpp>

#include <array>
#include <cstdint>

#include "pposix/any_view.hpp"

#define ANY_VIEW_TYPES uint8_t, short, uint32_t, int, uint64_t, long long, double, long double

TEMPLATE_TEST_CASE("any_view can be constructed", "[pposix][any_view]", ANY_VIEW_TYPES) {
  GIVEN(" a default constructed any_view") {
    pposix::any_view view{};

    THEN("the data pointer is null") { REQUIRE(view.data() == nullptr); }
    AND_THEN("the length is zero") { REQUIRE(view.length() == 0u); }

    AND_WHEN("getting a read-only byte span") {
      const auto bytes{view.as_bytes()};
      THEN("it also has a null data pointer") { REQUIRE(bytes.data() == nullptr); }
      AND_THEN("it also has a length of 0") { REQUIRE(bytes.length() == 0u); }
    }

    AND_WHEN("getting a writeable byte span") {
      const auto bytes{view.as_writeable_bytes()};
      THEN("it also has a null data pointer") { REQUIRE(bytes.data() == nullptr); }
      AND_THEN("it also has a length of 0") { REQUIRE(bytes.length() == 0u); }
    }
  }

  GIVEN("an any_view constructed with an object") {
    TestType object{};
    pposix::any_view view{&object};

    THEN("the data pointer is the same as the address of the object") {
      REQUIRE(view.data() == &object);
    }
    AND_THEN("the length is the same as the size of the object") {
      REQUIRE(view.length() == sizeof(object));
    }
  }

  GIVEN("an any_view constructed with a c-array") {
    TestType c_array[100u]{};
    pposix::any_view view{&c_array};

    THEN("the data pointer is the same as the starting address of the array") {
      REQUIRE(view.data() == &c_array);
      REQUIRE(view.data() == &c_array[0u]);
    }
    AND_THEN("the length is the same as the byte size of the array") {
      REQUIRE(view.length() == sizeof(c_array));
    }
  }

  GIVEN("an any_view constructed with a std::array") {
    std::array<TestType, 100u> std_array{};
    pposix::any_view view{&std_array};

    THEN("the data pointer is the same as the starting address of the array") {
      REQUIRE(view.data() == std_array.data());
      REQUIRE(view.data() == &std_array[0u]);
    }
    AND_THEN("the length is the same as the byte size of the array") {
      REQUIRE(view.length() == std_array.size() * sizeof(TestType));
    }
  }
}

TEMPLATE_TEST_CASE("any_cview can be constructed", "[pposix][any_view]", ANY_VIEW_TYPES) {
  GIVEN(" a default constructed any_cview") {
    pposix::any_cview view{};

    THEN("the data pointer is null") { REQUIRE(view.data() == nullptr); }
    AND_THEN("the length is zero") { REQUIRE(view.length() == 0u); }

    AND_WHEN("getting a read-only byte span") {
      const auto bytes{view.as_bytes()};
      THEN("it also has a null data pointer") { REQUIRE(bytes.data() == nullptr); }
      AND_THEN("it also has a length of 0") { REQUIRE(bytes.length() == 0u); }
    }
  }

  GIVEN("an any_cview constructed with an object") {
    const TestType object{};
    pposix::any_cview view{&object};

    THEN("the data pointer is the same as the address of the object") {
      REQUIRE(view.data() == &object);
    }
    AND_THEN("the length is the same as the size of the object") {
      REQUIRE(view.length() == sizeof(object));
    }
  }

  GIVEN("an any_cview constructed with a c-array") {
    const TestType c_array[100u]{};
    pposix::any_cview view{&c_array};

    THEN("the data pointer is the same as the starting address of the array") {
      REQUIRE(view.data() == &c_array);
      REQUIRE(view.data() == &c_array[0u]);
    }
    AND_THEN("the length is the same as the byte size of the array") {
      REQUIRE(view.length() == sizeof(c_array));
    }
  }

  GIVEN("an any_cview constructed with a std::array") {
    const std::array<const TestType, 100u> std_array{};
    pposix::any_cview view{&std_array};

    THEN("the data pointer is the same as the starting address of the array") {
      REQUIRE(view.data() == std_array.data());
      REQUIRE(view.data() == &std_array[0u]);
    }
    AND_THEN("the length is the same as the byte size of the array") {
      REQUIRE(view.length() == std_array.size() * sizeof(TestType));
    }
  }
}
