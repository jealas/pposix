#include "pposix_test.hpp"

#include "pposix/descriptor.hpp"

template <class T>
struct descriptor_tag {};

scenario_template("Descriptors can be used with pointer types", T, void*, int*, char*) {
  using descriptor_t = pposix::descriptor<descriptor_tag<T>, T, -1>;

  given("a default-constructed descriptor") {
    descriptor_t descriptor{};

    then("the descriptor is equal to the null descriptor") {
      require_eq(descriptor, pposix::null_descriptor);
    }
    and_then("the raw value is equal to the null value") {
      require_eq(descriptor.raw(), T(-1));
    }
    and_then("the raw value does not equal the 0 value") {
      require_ne(descriptor.raw(), T{});
    }
  }
}