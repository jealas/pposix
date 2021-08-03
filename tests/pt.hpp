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

enum class Type {
  Normal,
  Thread,
  Spawn,
};

struct RegistrationEntry {
  virtual ~RegistrationEntry() = default;

  RegistrationEntry(const Type type, const Id &id, const Location &location)
      : type{type}, id{id}, location{location} {}

  RegistrationEntry(const RegistrationEntry &) = delete;
  RegistrationEntry(RegistrationEntry &&) = delete;

  RegistrationEntry &operator=(const RegistrationEntry &) = delete;
  RegistrationEntry &operator=(RegistrationEntry &&) = delete;

  Type type{};
  Id id;
  Location location;

  RegistrationEntry const *next{};

  virtual void run_test() const;
};

void run(const std::vector<std::reference_wrapper<const Id>> &test_matches);

[[noreturn]] void main(const std::vector<std::regex> &patterns);

RegistrationEntry const *registered_tests();

class Test {
 public:
  inline explicit Test(RegistrationEntry const *entry) : entry_{entry} {
    assert(entry != nullptr);
  }

  inline const Id &id() const noexcept { return entry_->id; }
  inline const Location &loc() const noexcept { return entry_->location; }

  inline void run_test() const { entry_->run_test(); }

 private:
  RegistrationEntry const *entry_{};
};

void register_test(RegistrationEntry &entry);

template <class>
class Registration : public RegistrationEntry {
 public:
  Registration(const Type type, const Id &id, const Location &location) noexcept
      : RegistrationEntry{type, id, location} {
    ::pt::register_test(*this);
  }
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

#define PT_GENERIC_TEST(name_space, name, type)                                         \
  namespace name_space {                                                                \
  struct name : ::pt::Registration<name> {                                              \
    using ::pt::Registration<name>::Registration;                                       \
    void run_test() const override;                                                     \
  } const static name##_registration{type, {#name_space, #name}, {__FILE__, __LINE__}}; \
  }                                                                                     \
  void name_space ::name ::run_test() const

#define PT_TEST(name_space, name) PT_GENERIC_TEST(name_space, name, ::pt::Type::Normal)

#define PT_THREAD_TEST(name_space, name) PT_GENERIC_TEST(name_space, name, ::pt::Type::Thread)

#define PT_SPAWN_TEST(name_space, name) PT_GENERIC_TEST(name_space, name, ::pt::Type::Spawn)

#endif  // PPOSIX_PT_HPP
