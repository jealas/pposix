#ifndef PPOSIX_PT_HPP
#define PPOSIX_PT_HPP

#ifdef __cplusplus
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <system_error>
#include <vector>

#else
#include <stddef.h>
#include <stdint.h>

#endif

#ifdef __cplusplus
namespace pt {
namespace capi {
extern "C" {
#endif  // __cplusplus

#ifdef __cplusplus
#define PT_CAPI_NOEXCEPT noexcept
#define PT_CAPI_UNDERLYING_ENUM(type) : type
#else
#define PT_CAPI_NOEXCEPT
#define PT_CAPI_UNDERLYING_ENUM
#endif

struct PtTestEntry {
  void const *handle;
};

struct PtTestEntriesStop {
  int val;
};

struct PtTestEntriesCount {
  size_t val;
};

PtTestEntry pt_normal_tests() PT_CAPI_NOEXCEPT;

PtTestEntriesStop pt_test_entries_stop(PtTestEntry) PT_CAPI_NOEXCEPT;
PtTestEntry pt_test_entries_next(PtTestEntry) PT_CAPI_NOEXCEPT;

typedef int pt_test_type_t;

enum pt_test_type PT_CAPI_UNDERLYING_ENUM(pt_test_type_t){
    pt_normal_type,
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

enum pt_run_result PT_CAPI_UNDERLYING_ENUM(pt_run_result_t){
    run_success, run_skipped, run_failed, run_error, run_internal_error, run_exception,
};

struct PtTestRunResult {
  pt_run_result_t val;
};

PtTestType pt_test_entry_type(PtTestEntry) PT_CAPI_NOEXCEPT;
PtTestNamespace pt_test_entry_namespace(PtTestEntry) PT_CAPI_NOEXCEPT;
PtTestName pt_test_entry_name(PtTestEntry) PT_CAPI_NOEXCEPT;
PtTestFile pt_test_entry_file(PtTestEntry) PT_CAPI_NOEXCEPT;
PtTestLine pt_test_entry_line(PtTestEntry) PT_CAPI_NOEXCEPT;
PtTestRunResult pt_test_entry_run(PtTestEntry) PT_CAPI_NOEXCEPT;

struct PtSecret {
  uint8_t val[4];
};

struct PtVersion {
  uint8_t val[4];
};

#define PT_CAPI_SECRET \
  PtSecret { 'p', 't', 'l', 's' }

#define PT_CAPI_VERSION \
  PtVersion { 0, 0, 0, 0 }

struct PtSymbolTableId {
  PtSecret secret;
  PtVersion version;
};

struct PtSymbolTable {
  PtSymbolTableId id;

  PtTestEntry (*pt_normal_tests)() PT_CAPI_NOEXCEPT;
  PtTestEntriesStop (*pt_test_entries_stop)(PtTestEntry) PT_CAPI_NOEXCEPT;
  PtTestEntry (*pt_test_entries_next)(PtTestEntry) PT_CAPI_NOEXCEPT;

  PtTestType (*pt_test_entry_type)(PtTestEntry) PT_CAPI_NOEXCEPT;
  PtTestNamespace (*pt_test_entry_namespace)(PtTestEntry) PT_CAPI_NOEXCEPT;
  PtTestName (*pt_test_entry_name)(PtTestEntry) PT_CAPI_NOEXCEPT;
  PtTestFile (*pt_test_entry_file)(PtTestEntry) PT_CAPI_NOEXCEPT;
  PtTestLine (*pt_test_entry_line)(PtTestEntry) PT_CAPI_NOEXCEPT;
  PtTestRunResult (*pt_test_entry_run)(PtTestEntry) PT_CAPI_NOEXCEPT;
};

#define PT_SYMBOL_TABLE_NAME pt_symbol_table_65e13a5d_64ab_4214_8fd9_40478724a480
#define PT_SYMBOL_TABLE_NAME_STR "pt_symbol_table_65e13a5d_64ab_4214_8fd9_40478724a480"

#define PT_SYMBOL_TABLE_SIZE 9

#ifdef __cplusplus
static_assert(sizeof(PtSymbolTable) ==
                  (sizeof(PtSymbolTableId)) + (PT_SYMBOL_TABLE_SIZE * sizeof(void(*)())),
              "Symbol table size changed!");
#endif

#ifdef __cplusplus
}
}  // namespace capi
}  // namespace pt
#endif

#ifdef __cplusplus
namespace pt {

struct assert_line {
  char const *file{};
  size_t line{};
  char const *expression{};
};

struct internal_error final : std::runtime_error {
  using std::runtime_error::runtime_error;
};

namespace private_detail {

class test_exception {
 public:
  test_exception(std::string message) : message_{std::move(message)} {}

  inline const std::string &message() const noexcept { return message_; }

 private:
  std::string message_{};
};

}  // namespace private_detail

struct Location {
  capi::PtTestFile file;
  capi::PtTestLine line;

  inline std::string uri() const { return std::string{"file://"} + file.val; }
};

struct SubFail {
  pt::Location location{};
  const char *label{};
  const char *name{};
  std::string message{};
};

class test_failed final : public private_detail::test_exception {
 public:
  test_failed(std::string message, const assert_line &line)
      : test_exception{std::move(message)}, line_{line} {}

  inline const assert_line &line() const noexcept { return line_; }

  inline void push_sub_fail(const SubFail &fail) { subtest_fails_.emplace_back(fail); }

  inline const std::vector<SubFail> &subtest_fails() const { return subtest_fails_; }

 private:
  assert_line line_{};
  std::vector<SubFail> subtest_fails_{};
};

class test_skipped final : public private_detail::test_exception {
  using test_exception::test_exception;
};

inline void skip(std::string reason) { throw test_skipped{std::move(reason)}; }

struct Id {
  capi::PtTestNamespace name_space;
  capi::PtTestName name;

  std::string full_name() const { return std::string{name_space.val} + "::" + name.val; }
};

enum class TestType : capi::pt_test_type_t {
  Normal = capi::pt_test_type::pt_normal_type,
  Spawn = capi::pt_test_type::pt_spawn_type,
};

using test_fn = void() noexcept(false);

class InternalTest {
 public:
  InternalTest(test_fn &fn, const TestType t, const Id &id, const Location &loc) noexcept
      : fn_{fn}, type_{t}, id_{id}, location_{loc} {}

  inline void run() const noexcept(false) { return fn_(); }

  inline Id id() const noexcept { return id_; }
  inline Location loc() const noexcept { return location_; }
  inline TestType type() const noexcept { return type_; }

  // TODO: Make these private functions only for friends
  inline InternalTest const *next() const noexcept { return next_; }
  inline void set_next(InternalTest const *next) noexcept { next_ = next; }

 private:
  test_fn &fn_;
  TestType type_{};
  Id id_{};
  Location location_{};

  InternalTest const *next_{};
};

enum class RunResult : capi::pt_run_result_t {
  Success = capi::pt_run_result::run_success,
  Skipped = capi::pt_run_result::run_skipped,
  Failed = capi::pt_run_result::run_failed,
  Error = capi::pt_run_result::run_error,
  InternalError = capi::pt_run_result::run_internal_error,
  Exception = capi::pt_run_result::run_exception,
};

namespace private_detail {

RunResult run_internal(const InternalTest &test) noexcept;

void register_internal_test(TestType type, InternalTest &entry) noexcept;

InternalTest const *internal_tests() noexcept;

}  // namespace private_detail

template <class, TestType Type>
class Registration : public InternalTest {
 public:
  Registration(test_fn &fn, const Id &id, const Location &location) noexcept
      : InternalTest{fn, Type, id, location} {
    ::pt::private_detail::register_internal_test(Type, *this);
  }
};

template <class Result>
inline void assert_true(const Result &result, const assert_line &line) noexcept(false) {
  if (!result) {
    throw test_failed{"ASSERT FAILED", line};
  }
}

template <class T, class Fn>
inline void throws(const assert_line &line, const Fn &fn) noexcept(false) {
  try {
    fn();
  } catch (const T &) {
    return;
  } catch (const pt::private_detail::test_exception &) {
    throw;
  } catch (...) {
    throw test_failed{"Wrong exception was thrown", line};
  }

  throw test_failed{"No exception was thrown", line};
}

template <class T, class Fn>
inline void throws(const Fn &fn) noexcept(false) {
  throws<T>({}, fn);
}

namespace private_detail {

template <class Begin, class End>
struct subtest_runner {
  pt::Location location;
  char const *var_name{};
  Begin begin;
  End end;

  template <class Fn>
  auto &operator=(const Fn fn) noexcept(false) {
    for (; begin != end; ++begin) {
      try {
        fn(*begin);
      } catch (pt::test_failed &fail) {
        fail.push_sub_fail(pt::SubFail{location, "SUBTEST", var_name,
                                       std::string{var_name} + " = " + std::to_string(*begin)});
        throw;
      } catch (const pt::test_skipped &) {
        continue;
      }
    }

    return *this;
  }
};

}  // namespace private_detail

struct {
  template <class Begin, class End>
  constexpr auto from_iterators(const Location &location, const char *name, Begin begin,
                                End end) const noexcept {
    return private_detail::subtest_runner<Begin, End>{location, name, begin, end};
  }

  template <class Iterable>
  constexpr auto operator()(const Location &location, const char *name,
                            const Iterable &iterable) const noexcept {
    return from_iterators(location, name, std::begin(iterable), std::end(iterable));
  }

  template <class T>
  constexpr auto operator()(const ::pt::Location &location, char const *name,
                            const std::initializer_list<T> &iterable) const noexcept {
    return from_iterators(location, name, std::begin(iterable), std::end(iterable));
  }

  template <class Iterable>
  constexpr auto operator()(const Iterable &iterable) const noexcept(false) {
    return (*this)({}, "#UNK", iterable);
  }

  template <class T>
  constexpr auto operator()(const std::initializer_list<T> &iterable) const noexcept(false) {
    return (*this)({}, "#UNK", iterable);
  }

} constexpr subtest;

struct section_runner {
  pt::Location location;
  char const *name{};

  template <class Fn>
  auto &operator=(Fn fn) const noexcept(false) {
    try {
      fn();
    } catch (test_failed &fail) {
      fail.push_sub_fail(SubFail{location, "SECTION", name, ""});
      throw;
    }

    return *this;
  }
};

struct {
  constexpr section_runner operator()(const Location &location, char const *name) const noexcept {
    return section_runner{location, name};
  }

  constexpr section_runner operator()(char const *name) const noexcept {
    return section_runner{{}, name};
  }

} constexpr section;

}  // namespace pt

#define PT_ASSERTION_LINE_W_EXPRESSION(expression) \
  { __FILE__, __LINE__, expression }

#define PT_ASSERTION_LINE \
  { __FILE__, __LINE__ }

#define PT_SUITE(name_space)                \
  namespace {                               \
  struct {                                  \
    char const *namespace_str{#name_space}; \
    size_t line{__LINE__};                  \
  } static constexpr pt_test_suite;         \
  }                                         \
  namespace

#define PT_GENERIC_TEST(name, type)                                                   \
  struct name : ::pt::Registration<name, type> {                                      \
    using ::pt::Registration<name, type>::Registration;                               \
    static void run() noexcept(false);                                                \
  } const static name##_registration{                                                 \
      name::run, {{pt_test_suite.namespace_str}, {#name}}, {{__FILE__}, {__LINE__}}}; \
                                                                                      \
  void name::run() noexcept(false)

#define PT_TEST(name) PT_GENERIC_TEST(name, ::pt::TestType::Normal)

#define PT_SPAWN(name) PT_GENERIC_TEST(name, ::pt::TestType::Spawn)

#define PT_ASSERT(expression) \
  ::pt::assert_true((expression), PT_ASSERTION_LINE_W_EXPRESSION(#expression))

#define PT_THROWS(expression, exception)                               \
  ::pt::throws<exception>(PT_ASSERTION_LINE_W_EXPRESSION(#expression), \
                          [&]() { (void)(expression); })

#define PT_LOCATION          \
  ::pt::Location {           \
    {__FILE__}, { __LINE__ } \
  }

#define PT_SUBTEST(var, ...) \
  ::pt::subtest(PT_LOCATION, #var, __VA_ARGS__) = [&](const auto &var) noexcept(false)

#define PT_SECTION(name) ::pt::section(PT_LOCATION, #name) = [&]() noexcept(false)

#endif  // __cplusplus

#endif  // PPOSIX_PT_HPP
