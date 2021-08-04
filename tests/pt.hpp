#ifndef PPOSIX_PT_HPP
#define PPOSIX_PT_HPP

#ifdef __cplusplus
// Include the iostream header first
#include <iostream>
// DO NOT MOVE

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
#else
#include <stddef.h>
#include <stdint.h>
#endif

#include <cassert>
#include <functional>
#include <regex>
#include <thread>
#include <vector>
#endif  // __cplusplus

#ifdef __cplusplus
namespace pt::capi {
extern "C" {
#endif  // __cplusplus

struct PtTestEntry {
  void const *handle;
};

struct PtTestEntryStop {
  int val;
};

PtTestEntry pt_test_entries() noexcept;

PtTestEntryStop pt_test_entries_stop(PtTestEntry) noexcept;
PtTestEntry pt_test_entries_next(PtTestEntry) noexcept;

typedef int pt_test_type_t;

enum pt_test_type : pt_test_type_t {
  pt_normal_type,
  pt_thread_type,
  pt_spawn_type,
};

struct PtTestType {
  pt_test_type_t val;
};

struct PtTestNamespace {
  char const *val;
};

struct PtTestName {
  char const *val;
};

struct PtTestFile {
  char const *val;
};

struct PtTestLine {
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

struct PtTestRunResult {
  pt_run_result_t val;
};

PtTestType pt_test_entry_type(PtTestEntry) noexcept;
PtTestNamespace pt_test_entry_namespace(PtTestEntry) noexcept;
PtTestName pt_test_entry_name(PtTestEntry) noexcept;
PtTestFile pt_test_entry_file(PtTestEntry) noexcept;
PtTestLine pt_test_entry_line(PtTestEntry) noexcept;
PtTestRunResult pt_test_entry_run(PtTestEntry) noexcept;

#define PT_CAPI_SECRET \
  { 'p', 't', 'l', 's' }

#define PT_CAPI_VERSION \
  { 0, 0, 0, 0 }

struct PtSymbolTableId {
  uint8_t secret[4];
  uint8_t version[4];
};

struct PtSymbolTable {
  PtSymbolTableId id;

  PtTestEntry (*pt_test_entries)() noexcept;
  PtTestEntryStop (*pt_test_entries_stop)(PtTestEntry) noexcept;
  PtTestEntry (*pt_test_entries_next)(PtTestEntry) noexcept;

  PtTestType (*pt_test_entry_type)(PtTestEntry) noexcept;
  PtTestNamespace (*pt_test_entry_namespace)(PtTestEntry) noexcept;
  PtTestName (*pt_test_entry_name)(PtTestEntry) noexcept;
  PtTestFile (*pt_test_entry_file)(PtTestEntry) noexcept;
  PtTestLine (*pt_test_entry_line)(PtTestEntry) noexcept;
  PtTestRunResult (*pt_test_entry_run)(PtTestEntry) noexcept;
};

#ifdef __cplusplus
static_assert(sizeof(PtSymbolTable) == (sizeof(PtSymbolTableId)) + (9 * sizeof(void(*)())));
#endif

#ifdef __cplusplus
}
}  // namespace pt::capi
#endif

#ifdef __cplusplus
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
  capi::PtTestNamespace name_space{};
  capi::PtTestName name{};

  std::string full_name() const { return std::string{name_space.val} + "::" + name.val; }
};

inline std::ostream &operator<<(std::ostream &out, const Id &id) {
  return out << id.name_space.val << "::" << id.name.val;
}

struct Location {
  capi::PtTestFile file{};
  capi::PtTestLine line{};

  inline std::string uri() const { return std::string{"file://"} + file.val; }
};

inline std::ostream &operator<<(std::ostream &out, const Location &location) {
  return out << location.file.val << ':' << location.line.val;
}

enum class Type : capi::pt_test_type_t {
  Normal = capi::pt_test_type::pt_normal_type,
  Thread = capi::pt_test_type::pt_thread_type,
  Spawn = capi::pt_test_type::pt_spawn_type,
};

struct Test {
  virtual ~Test() = default;

  Test() noexcept = default;

  Test(const Test &) = delete;
  Test(Test &&) = delete;

  Test &operator=(const Test &) = delete;
  Test &operator=(Test &&) = delete;

  virtual Id id() const noexcept = 0;
  virtual Location loc() const noexcept = 0;
  virtual Type type() const noexcept = 0;

  virtual void run() const = 0;
};

class InternalTest : public Test {
 public:
  InternalTest(const Type t, const Id &id, const Location &loc) noexcept
      : type_{t}, id_{id}, location_{loc} {}

  inline Id id() const noexcept { return id_; }
  inline Location loc() const noexcept { return location_; }
  inline Type type() const noexcept { return type_; }

  virtual void run() const;

  // TODO: Make these private functions only for friends
  inline InternalTest const *next() const noexcept { return next_; }
  inline void set_next(InternalTest const *next) noexcept { next_ = next; }

 private:
  Type type_{};
  Id id_{};
  Location location_{};

  InternalTest const *next_{};
};

class ExternalTest : public Test {
 public:
  inline explicit ExternalTest(capi::PtSymbolTable syms, capi::PtTestEntry test)
      : syms_{syms}, test_{test} {};

  inline Id id() const noexcept {
    return Id{syms_.pt_test_entry_namespace(test_), syms_.pt_test_entry_name(test_)};
  }

  inline Location loc() const noexcept {
    return Location{syms_.pt_test_entry_file(test_), syms_.pt_test_entry_line(test_)};
  }

  inline Type type() const noexcept { return Type{syms_.pt_test_entry_type(test_).val}; }

  inline virtual void run() const { syms_.pt_test_entry_run(test_); }

 private:
  capi::PtSymbolTable syms_{};
  capi::PtTestEntry test_;
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

#define PT_GENERIC_TEST(name_space, name, type)                                                 \
  namespace name_space {                                                                        \
  struct name : ::pt::Registration<name> {                                                      \
    using ::pt::Registration<name>::Registration;                                               \
    void run() const override;                                                                  \
  } const static name##_registration{type, {{#name_space}, {#name}}, {{__FILE__}, {__LINE__}}}; \
  }                                                                                             \
  void name_space ::name ::run() const

#define PT_TEST(name_space, name) PT_GENERIC_TEST(name_space, name, ::pt::Type::Normal)

#define PT_THREAD_TEST(name_space, name) PT_GENERIC_TEST(name_space, name, ::pt::Type::Thread)

#define PT_SPAWN_TEST(name_space, name) PT_GENERIC_TEST(name_space, name, ::pt::Type::Spawn)

#endif  // __cplusplus

#endif  // PPOSIX_PT_HPP
