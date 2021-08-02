#ifndef PPOSIX_PPOSIX_TEST_HPP
#define PPOSIX_PPOSIX_TEST_HPP

// Include the iostream header first
#include <iostream>
// DO NOT MOVE

#include <functional>
#include <regex>
#include <thread>
#include <unordered_map>
#include <vector>

namespace pposix::test {

struct Info {
  char const *name_space{};
  char const *name{};
  std::function<void()> fn{};

  std::string full_name() const { return std::string{name_space} + "::" + name; }
};

inline std::ostream &operator<<(std::ostream &out, const Info &info) {
  return out << info.name_space << "::" << info.name;
}

struct assertion_error : std::runtime_error {
  using runtime_error::runtime_error;
};

void register_test(char const *name_space, char const *name, std::function<void()> test_fn);

const std::unordered_map<std::string, Info> &registered_tests();

template <class TestFn>
struct Registration {
  Registration(char const *const name_space, char const *const name, const TestFn &fn) noexcept {
    ::pposix::test::register_test(name_space, name, std::reference_wrapper(fn));
  }
};

void run(const std::vector<std::reference_wrapper<const Info>> &test_matches);

[[noreturn]] void main(const std::vector<std::regex> &patterns);

struct assert_line {
  char const *file;
  size_t line;
  char const *expression;
};

template <class Result>
inline void assert_true(const Result &result, const assert_line &line) {
  if (!result) {
    throw assertion_error{std::string{"Assertion "} + line.expression + " failed @ " +
                          std::string{line.file} + ":" + std::to_string(line.line)};
  }
}

}  // namespace pposix::test

#define PPOSIX_ASSERT(expression) \
  ::pposix::test::assert_true((expression), {__FILE__, __LINE__, #expression})

#define PPOSIX_TEST(name_space, name, test_fn)                 \
  namespace name_space {                                       \
  static constexpr auto name##_fn{test_fn};                    \
  struct : ::pposix::test::Registration<decltype(name##_fn)> { \
    using Registration::Registration;                          \
  } static name##_registration{#name_space, #name, name##_fn}; \
  }

#endif  // PPOSIX_PPOSIX_TEST_HPP
