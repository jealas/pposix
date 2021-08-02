#ifndef PPOSIX_PPOSIX_TEST_HPP
#define PPOSIX_PPOSIX_TEST_HPP

#include <functional>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

namespace pposix::test {

struct TestInfo {
  char const *name{};
  char const *file{};
  size_t line{};
  std::function<void()> fn{};
};

struct assertion_error : std::runtime_error {
  using runtime_error::runtime_error;
};

struct test_error : std::runtime_error {
  using runtime_error::runtime_error;
};

struct {
  std::unordered_map<std::type_index, TestInfo> tests{};
} registrar{};

template <class TestFn>
struct Registration {
  Registration(char const *const name, char const *const file, size_t line,
               const TestFn &fn) noexcept {
    try {
      registrar.tests.emplace(
          std::make_pair(std::type_index(typeid(TestFn)),
                         TestInfo{name, file, line, std::reference_wrapper(fn)}));
    } catch (const std::exception &exception) {
      std::cerr << "Uncaught exception while registering test " << file << ':' << line << " - "
                << exception.what() << '\n';
      std::cerr << std::endl;
      std::exit(EXIT_FAILURE);
    } catch (...) {
      std::cerr << "Unknown exception caught while registering test " << file << ':' << line
                << '\n';
      std::cerr << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }
};

void main() {
  for (const auto &elements : pposix::test::registrar.tests) {
    const auto &test_info{elements.second};

    try {
      std::cout << "Running " << test_info.name << '\n';
      test_info.fn();

    } catch (const assertion_error &error) {
      std::cerr << "FAILED " << test_info.name << " " << test_info.file << ':' << test_info.line
                << '\n'
                << error.what();
      std::cerr << std::endl;
      continue;

    } catch (const std::exception &exception) {
      std::cerr << "Uncaught exception while registering test " << test_info.file << ':'
                << test_info.line << '\n'
                << exception.what();

      std::cerr << std::endl;
      std::exit(EXIT_FAILURE);
    } catch (...) {
      std::cerr << "Unknown exception caught while registering test " << test_info.file << ':'
                << test_info.line;

      std::cerr << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }
}

struct assert_line {
  char const *file;
  size_t line;
  char const *expression;
};

template <class Result>
inline void assert(const Result &result, const assert_line &line) {
  if (!result) {
    throw assertion_error{std::string{"Assertion "} + line.expression + " failed @ " +
                          std::string{line.file} + ":" + std::to_string(line.line)};
  }
}

}  // namespace pposix::test

#define PPOSIX_ASSERT(expression) \
  ::pposix::test::assert((expression), {__FILE__, __LINE__, #expression})

#define PPOSIX_TEST(name_space, name, test_fn)                   \
  namespace name_space {                                         \
  static const auto name{test_fn};                               \
  struct : ::pposix::test::Registration<decltype(name)> {        \
    using Registration::Registration;                            \
  } static name##_registration{#name, __FILE__, __LINE__, name}; \
  }

#endif  // PPOSIX_PPOSIX_TEST_HPP
