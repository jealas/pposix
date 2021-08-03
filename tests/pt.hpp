#ifndef PPOSIX_PT_HPP
#define PPOSIX_PT_HPP

// Include the iostream header first
#include <iostream>
// DO NOT MOVE

#include <cassert>
#include <functional>
#include <regex>
#include <thread>
#include <vector>

namespace pt {

struct assert_line {
  char const *file;
  size_t line;
  char const *expression;
};

struct test_failed : std::runtime_error {
  using runtime_error::runtime_error;
};

class assertion_failure : public test_failed {
 public:
  assertion_failure(const std::string &message, const assert_line &line)
      : test_failed{message}, line_{line} {}

  const assert_line &line() const noexcept { return line_; }

 private:
  assert_line line_{};
};

class test_skipped : std::runtime_error {
  using std::runtime_error::runtime_error;
};

inline void skip(std::string reason) { throw test_skipped{std::move(reason)}; }

struct stop_iteration : std::exception {
  using std::exception::exception;
};

template <class Start, class End>
constexpr auto range(Start start, End end) {
  return [start = start, end = end]() mutable {
    if (start < end) {
      return start++;
    } else {
      throw stop_iteration{};
    }
  };
}

template <class Iterable>
constexpr auto range(const Iterable &iterable) {
  return range(std::cbegin(iterable), std::cend(iterable));
}

struct Id {
  char const *name_space{};
  char const *name{};

  std::string full_name() const { return std::string{name_space} + "::" + name; }
};

inline std::ostream &operator<<(std::ostream &out, const Id &id) {
  return out << id.name_space << "::" << id.name;
}

struct Location {
  char const *file{};
  size_t line{};

  inline std::string uri() const { return std::string{"file://"} + file; }
};

inline std::ostream &operator<<(std::ostream &out, const Location &location) {
  return out << location.file << ':' << location.line;
}

struct RegistrationEntry {
  virtual ~RegistrationEntry() = default;

  RegistrationEntry(const Id &id, const Location &location) : id{id}, location{location} {}

  RegistrationEntry(const RegistrationEntry &) = delete;
  RegistrationEntry(RegistrationEntry &&) = delete;

  RegistrationEntry &operator=(const RegistrationEntry &) = delete;
  RegistrationEntry &operator=(RegistrationEntry &&) = delete;

  std::reference_wrapper<const Id> id;
  std::reference_wrapper<const Location> location;

  RegistrationEntry const *next{};

  virtual void run() const;
};

struct Test {
  inline explicit Test(RegistrationEntry const *entry) : entry_{entry} {
    assert(entry != nullptr);
  }

  inline const Id &id() const noexcept { return entry_->id; }

  inline const Location &loc() const noexcept { return entry_->location; }

  inline void run() const { return entry_->run(); }

  RegistrationEntry const *entry_{};
};

template <class Iterator, class Body>
void subtest(Iterator iter, Body body) {
  while (true) {
    try {
      body(iter());
    } catch (const pt::test_skipped &) {
      continue;
    } catch (const pt::stop_iteration &) {
      break;
    }
  }
}

#define PT_SUBTEST2(name, iterable)  for (for )

#define PT_SUBTEST(name, iterable)             \
  void subtest_##__LINE__();           \
                                               \
  ::pt::subtest(iterable, subtest_##__LINE__); \
                                               \
  void subtest_##__LINE__()

void register_test(RegistrationEntry &entry);

template <class>
class Registration : public RegistrationEntry {
 public:
  Registration(const Id &id, const Location &location) noexcept : RegistrationEntry{id, location} {
    ::pt::register_test(*this);
  }
};

void run(const std::vector<std::reference_wrapper<const Id>> &test_matches);

[[noreturn]] void main(const std::vector<std::regex> &patterns);

RegistrationEntry const *registered_tests();

template <class Result>
inline void assert_true(const Result &result, const assert_line &line) {
  if (!result) {
    throw assertion_failure{std::string{line.expression} + " was false @ " +
                                std::string{line.file} + ':' + std::to_string(line.line),
                            line};
  }
}

}  // namespace pt

#define PT_ASSERT(expression) ::pt::assert_true((expression), {__FILE__, __LINE__, #expression})

#define PT_TEST(name_space, name)                                 \
  namespace name_space {                                          \
  static constexpr ::pt::Location name##_loc{__FILE__, __LINE__}; \
  static constexpr ::pt::Id name##_id{#name_space, #name};        \
                                                                  \
  struct name##_t : ::pt::Registration<name##_t> {                \
    using ::pt::Registration<name##_t>::Registration;             \
                                                                  \
    void run() const override;                                    \
  } const static name##_registration{name##_id, name##_loc};      \
  }                                                               \
  void name_space ::name##_t::run() const
#endif  // PPOSIX_PT_HPP
