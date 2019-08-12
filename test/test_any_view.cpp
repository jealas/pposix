#include "pposix_test.hpp"

#include <array>
#include <cstdint>

#include "pposix/any_view.hpp"

#define ANY_VIEW_TYPES uint8_t, short, uint32_t, int, uint64_t, long long, double, long double

scenario_template("any_view can be constructed", TestType, ANY_VIEW_TYPES) {
  subcase(" a default constructed any_view") {
    pposix::any_view view{};

    then("the data pointer is null") { require(view.data() == nullptr); }
    and_then("the length is zero") { require(view.length() == 0u); }
    and_then("it is empty") { require(view.empty()); }

    and_when("getting a read-only byte span") {
      const auto bytes{view.as_bytes()};
      then("it also has a null data pointer") { require(bytes.data() == nullptr); }
      and_then("it also has a length of 0") { require(bytes.length() == 0u); }
    }

    and_when("getting a writeable byte span") {
      const auto bytes{view.as_writeable_bytes()};
      then("it also has a null data pointer") { require(bytes.data() == nullptr); }
      and_then("it also has a length of 0") { require(bytes.length() == 0u); }
    }
  }

  given("an any_view constructed with an object") {
    TestType object{};
    pposix::any_view view{&object};

    then("the data pointer is the same as the address of the object") {
      require(view.data() == &object);
    }
    and_then("the length is the same as the size of the object") {
      require(view.length() == sizeof(object));
    }

    and_then("it is not empty") { require(not view.empty()); }
  }

  given("an any_view constructed with a c-array") {
    TestType c_array[100u]{};
    pposix::any_view view{&c_array};

    then("the data pointer is the same as the starting address of the array") {
      require(view.data() == &c_array);
      require(view.data() == &c_array[0u]);
    }
    and_then("the length is the same as the byte size of the array") {
      require(view.length() == sizeof(c_array));
    }
    and_then("it is not empty") { require(not view.empty()); }
  }

  given("an any_view constructed with a std::array") {
    std::array<TestType, 100u> std_array{};
    pposix::any_view view{&std_array};

    then("the data pointer is the same as the starting address of the array") {
      require(view.data() == std_array.data());
      require(view.data() == &std_array[0u]);
    }
    and_then("the length is the same as the byte size of the array") {
      require(view.length() == std_array.size() * sizeof(TestType));
    }
    and_then("it is not empty") { require(not view.empty()); }
  }
}

scenario_template("any_cview can be constructed", TestType, ANY_VIEW_TYPES) {
  given(" a default constructed any_cview") {
    pposix::any_cview view{};

    then("the data pointer is null") { require(view.data() == nullptr); }
    and_then("the length is zero") { require(view.length() == 0u); }

    and_when("getting a read-only byte span") {
      const auto bytes{view.as_bytes()};
      then("it also has a null data pointer") { require(bytes.data() == nullptr); }
      and_then("it also has a length of 0") { require(bytes.length() == 0u); }
      and_then("it is empty") { require(view.empty()); }
    }
  }

  given("an any_cview constructed with an object") {
    const TestType object{};
    pposix::any_cview view{&object};

    then("the data pointer is the same as the address of the object") {
      require(view.data() == &object);
    }
    and_then("the length is the same as the size of the object") {
      require(view.length() == sizeof(object));
    }
    and_then("it is not empty") { require(not view.empty()); }
  }

  given("an any_cview constructed with a c-array") {
    const TestType c_array[100u]{};
    pposix::any_cview view{&c_array};

    then("the data pointer is the same as the starting address of the array") {
      require(view.data() == &c_array);
      require(view.data() == &c_array[0u]);
    }
    and_then("the length is the same as the byte size of the array") {
      require(view.length() == sizeof(c_array));
    }
    and_then("it is not empty") { require(not view.empty()); }
  }

  given("an any_cview constructed with a std::array") {
    const std::array<const TestType, 100u> std_array{};
    pposix::any_cview view{&std_array};

    then("the data pointer is the same as the starting address of the array") {
      require(view.data() == std_array.data());
      require(view.data() == &std_array[0u]);
    }
    and_then("the length is the same as the byte size of the array") {
      require(view.length() == std_array.size() * sizeof(TestType));
    }
    and_then("it is not empty") { require(not view.empty()); }
  }
}
