#include <catch2/catch.hpp>

#include <system_error>

#include "pposix/descriptor.hpp"
#include "pposix/unique_descriptor.hpp"

template <class Descriptor>
struct null_close {
  std::error_code operator()(Descriptor) const noexcept { return {}; }
};

TEMPLATE_TEST_CASE("Unique int descriptors can be default constructed",
                   "[pposix][unique_descriptor]", int, short, long, long long) {
  using fd_t = pposix::descriptor<TestType, -1>;
  pposix::unique_descriptor<fd_t, null_close<fd_t>> descriptor{};

  REQUIRE(descriptor.empty());
  REQUIRE(not descriptor);
}
TEMPLATE_TEST_CASE("Unique ptr descriptors can be default constructed",
                   "[pposix][unique_descriptor]", int*, short*, long*, long long*) {
  using fd_t = pposix::descriptor<TestType, nullptr>;
  pposix::unique_descriptor<fd_t, null_close<fd_t>> descriptor{};

  REQUIRE(descriptor.empty());
  REQUIRE(not descriptor);
}
