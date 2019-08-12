#include "pposix_test.hpp"

#include <system_error>

#include "pposix/descriptor.hpp"
#include "pposix/unique_descriptor.hpp"

template <class Descriptor>
struct null_close {
  std::error_code operator()(Descriptor) const noexcept { return {}; }
};

test_case_template("Unique int descriptors can be default constructed", TestType, int, short, long,
                   long long) {
  using fd_t = pposix::descriptor<TestType, -1>;
  pposix::unique_descriptor<fd_t, null_close<fd_t>> descriptor{};

  require(descriptor.empty());
  require(not descriptor);
}
test_case_template("Unique ptr descriptors can be default constructed", TestType, int*, short*,
                   long*, long long*) {
  using fd_t = pposix::descriptor<TestType, nullptr>;
  pposix::unique_descriptor<fd_t, null_close<fd_t>> descriptor{};

  require(descriptor.empty());
  require(not descriptor);
}
