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

enum class Type : int {
  Normal,
  Thread,
  Spawn,
};

struct Test {
  virtual ~Test() = default;

  Test() noexcept = default;

  Test(const Test &) = delete;
  Test(Test &&) = delete;

  Test &operator=(const Test &) = delete;
  Test &operator=(Test &&) = delete;

  virtual const Id &id() const noexcept = 0;
  virtual const Location &loc() const noexcept = 0;
  virtual Type type() const noexcept = 0;

  virtual void run() const = 0;
};

class InternalTest : public Test {
 public:
  InternalTest(const Type t, const Id &id, const Location &loc) noexcept
      : type_{t}, id_{id}, location_{loc} {}

  inline const Id &id() const noexcept { return id_; }
  inline const Location &loc() const noexcept { return location_; }
  inline Type type() const noexcept { return type_; }

  virtual void run() const;

  inline InternalTest const *next() const noexcept { return next_; }

  // TODO: Make this a private function only for friends
  inline void set_next(InternalTest const *next) noexcept { next_ = next; }

 private:
  Type type_{};
  Id id_{};
  Location location_{};

  InternalTest const *next_{};
};

void run(const Test &test) noexcept;

namespace private_detail {

void register_internal_test(InternalTest &entry) noexcept;

InternalTest const *internal_tests() noexcept;
}  // namespace private_detail

template <class>
class Registration : public InternalTest {
 public:
  Registration(const Type type, const Id &id, const Location &location) noexcept
      : InternalTest{type, id, location} {
    ::pt::private_detail::register_internal_test(*this);
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
    void run() const override;                                                          \
  } const static name##_registration{type, {#name_space, #name}, {__FILE__, __LINE__}}; \
  }                                                                                     \
  void name_space ::name ::run() const

#define PT_TEST(name_space, name) PT_GENERIC_TEST(name_space, name, ::pt::Type::Normal)

#define PT_THREAD_TEST(name_space, name) PT_GENERIC_TEST(name_space, name, ::pt::Type::Thread)

#define PT_SPAWN_TEST(name_space, name) PT_GENERIC_TEST(name_space, name, ::pt::Type::Spawn)

namespace pt::capi {

struct Entry {
  void const *handle;
};

extern "C" {

struct Stop {
  int val;
};

Entry pt_test_entries() noexcept;

Stop pt_test_entries_stop(Entry) noexcept;
Entry pt_test_entries_next(Entry) noexcept;

typedef int pt_test_type_t;

enum pt_test_type : pt_test_type_t {
  pt_normal_type,
  pt_thread_type,
  pt_spawn_type,
};

struct Type {
  pt_test_type_t val;
};

struct Namespace {
  char const *val;
};

struct Name {
  char const *val;
};

struct File {
  char const *val;
};

struct Line {
  size_t val;
};

typedef int pt_run_result_t;

enum pt_run_result : pt_run_result_t {
  run_success,
  run_skipped,
  run_failed,
  run_error,
  run_internal_error,
};

struct RunResult {
  pt_run_result_t val;
};

Type pt_test_entry_type(Entry) noexcept;
Namespace pt_test_entry_namespace(Entry) noexcept;
Name pt_test_entry_name(Entry) noexcept;
File pt_test_entry_file(Entry) noexcept;
Line pt_test_entry_line(Entry) noexcept;
RunResult pt_test_entry_run(Entry) noexcept;
}

struct symbol_table {
  Entry (*pt_test_entries)() noexcept;
  Stop (*pt_test_entries_stop)(Entry) noexcept;
  Entry (*pt_test_entries_next)(Entry) noexcept;

  Type (*pt_test_entry_type)(Entry) noexcept;
  Namespace (*pt_test_entry_namespace)(Entry) noexcept;
  Name (*pt_test_entry_name)(Entry) noexcept;
  File (*pt_test_entry_file)(Entry) noexcept;
  Line (*pt_test_entry_line)(Entry) noexcept;
  RunResult (*pt_test_entry_run)(Entry) noexcept;
};

}  // namespace pt::capi

#endif  // PPOSIX_PT_HPP
