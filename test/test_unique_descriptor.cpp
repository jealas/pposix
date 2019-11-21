#include "pposix_test.hpp"

#include <system_error>

#include "pposix/descriptor.hpp"
#include "pposix/unique_descriptor.hpp"

template <class Descriptor>
struct null_close {
  std::error_code operator()(Descriptor) const noexcept { return {}; }
};

struct descriptor_tag {};

template <class T>
struct null_value {
  T operator()() const noexcept { return T(-1); }
};

test_case_template("Unique int descriptors can be default constructed", TestType, int, short, long,
                   long long) {
  using fd_t = pposix::descriptor<descriptor_tag, TestType, null_value<TestType>>;
  pposix::unique_descriptor<fd_t, null_close<fd_t>> descriptor{};

  require(descriptor.empty());
  require(not descriptor);
}

template <class T>
struct nullptr_value {
  T operator()() const noexcept { return nullptr; }
};

test_case_template("Unique ptr descriptors can be default constructed", TestType, int*, short*,
                   long*, long long*) {
  using fd_t = pposix::descriptor<descriptor_tag, TestType, nullptr_value<TestType>>;
  pposix::unique_descriptor<fd_t, null_close<fd_t>> descriptor{};

  require(descriptor.empty());
  require(not descriptor);
}
