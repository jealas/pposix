#ifndef PPOSIX_PT_HPP
#define PPOSIX_PT_HPP

#ifdef __cplusplus
#include <cassert>
#include <cstddef>
#include <cstdint>
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

typedef int pt_log_level_t;

enum pt_log_level PT_CAPI_UNDERLYING_ENUM(pt_log_level_t){
    info,
    warning,
    error,
    fatal,
};

struct PtLogLevel {
  pt_log_level val;
};

struct PtStreamData {
  void *val;
};

struct PtStreamVTable {
  void (*write)(PtStreamData data, char const *const);
};

struct PtLogStream {
  PtStreamVTable const *vtable;
  PtStreamData stream;
};

struct PtLoggerData {
  void *val;
};

struct PtLoggerVTable {
  void (*log)(PtLoggerData, PtLogLevel, char const *const);
  PtLogStream (*stream)(PtLoggerData, PtLogLevel);
};

struct PtLogger {
  PtLoggerVTable const *vtable;
  PtLoggerData logger;
};

typedef int pt_run_result_t;

enum pt_run_result PT_CAPI_UNDERLYING_ENUM(pt_run_result_t){
    run_success, run_skipped, run_failed, run_error, run_internal_error, run_exception,
};

struct PtTestRunResult {
  pt_run_result_t val;
};

PtTestNamespace pt_test_entry_namespace(PtTestEntry) PT_CAPI_NOEXCEPT;
PtTestName pt_test_entry_name(PtTestEntry) PT_CAPI_NOEXCEPT;
PtTestFile pt_test_entry_file(PtTestEntry) PT_CAPI_NOEXCEPT;
PtTestLine pt_test_entry_line(PtTestEntry) PT_CAPI_NOEXCEPT;
PtTestRunResult pt_test_entry_run(PtTestEntry, PtLogger) PT_CAPI_NOEXCEPT;

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

  PtTestNamespace (*pt_test_entry_namespace)(PtTestEntry) PT_CAPI_NOEXCEPT;
  PtTestName (*pt_test_entry_name)(PtTestEntry) PT_CAPI_NOEXCEPT;
  PtTestFile (*pt_test_entry_file)(PtTestEntry) PT_CAPI_NOEXCEPT;
  PtTestLine (*pt_test_entry_line)(PtTestEntry) PT_CAPI_NOEXCEPT;
  PtTestRunResult (*pt_test_entry_run)(PtTestEntry, PtLogger) PT_CAPI_NOEXCEPT;
};

#define PT_SYMBOL_TABLE_NAME pt_symbol_table_65e13a5d_64ab_4214_8fd9_40478724a480
#define PT_SYMBOL_TABLE_NAME_STR "pt_symbol_table_65e13a5d_64ab_4214_8fd9_40478724a480"

#define PT_SYMBOL_TABLE_SIZE 8

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

enum class LogLevel : capi::pt_log_level_t {
  Info = capi::pt_log_level::info,
  Warn = capi::pt_log_level::warning,
  Error = capi::pt_log_level::error,
  Fatal = capi::pt_log_level::fatal,
};

class LogStream {
 public:
  inline explicit LogStream(capi::PtLogStream stream) : stream_{stream} {}

  inline void write(char const *const str) const noexcept {
    stream_.vtable->write(stream_.stream, str);
  }

 private:
  capi::PtLogStream stream_;
};

inline LogStream &operator<<(LogStream &stream, char const *const str) {
  stream.write(str);
  return stream;
}

inline LogStream &operator<<(LogStream &stream, const std::string str) {
  stream.write(str.c_str());
  return stream;
}

inline LogStream &operator<<(LogStream &stream, const char c) {
  char buffer[2]{c, '\0'};
  stream.write(buffer);
  return stream;
}

inline LogStream &operator<<(LogStream &stream, const size_t size) {
  // TODO: Only allocate exactly the space necessary
  char buffer[100];
  std::sprintf(buffer, "%zu", size);

  stream.write(buffer);
  return stream;
}

class Logger {
 public:
  inline explicit Logger(capi::PtLogger logger) noexcept : logger_{logger} {}

  inline void log(char const *const line) const noexcept {
    // TODO: Replace
    logger_.vtable->log(logger_.logger, {capi::pt_log_level::info}, line);
  }

  inline void log(const std::string &line) const noexcept {
    logger_.vtable->log(logger_.logger, {capi::pt_log_level::info}, line.c_str());
  }

  inline LogStream stream(const LogLevel level) noexcept {
    return LogStream{
        logger_.vtable->stream(logger_.logger, {static_cast<capi::pt_log_level>(level)})};
  }

  inline capi::PtLogger c_log() const noexcept { return logger_; }

 private:
  capi::PtLogger logger_{};
};

struct Id {
  capi::PtTestNamespace name_space;
  capi::PtTestName name;

  std::string full_name() const { return std::string{name_space.val} + "::" + name.val; }
};

struct Location {
  capi::PtTestFile file;
  capi::PtTestLine line;

  inline std::string uri() const { return std::string{"file://"} + file.val; }
};

struct AssertLine {
  Location location{};
  char const *expression{};
};

inline LogStream &operator<<(LogStream &out, const Id &id) {
  return out << id.name_space.val << "::" << id.name.val;
}

inline LogStream &operator<<(LogStream &out, const Location &location) {
  return out << location.file.val << ":" << location.line.val;
}

inline LogStream &operator<<(LogStream &out, const AssertLine &line) {
  if (line.expression) {
    out << line.expression << (line.location.file.val ? " @ " : "");
  }

  if (line.location.file.val) {
    out << line.location.file.val << ":" << line.location.line.val;
  }

  return out;
}

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

struct SubFail {
  pt::Location location{};
  const char *label{};
  const char *name{};
  std::string message{};
};

class test_failed final : public private_detail::test_exception {
 public:
  test_failed(std::string message, const AssertLine &line)
      : test_exception{std::move(message)}, line_{line} {}

  inline const AssertLine &line() const noexcept { return line_; }

  inline void push_sub_fail(const SubFail &fail) { subtest_fails_.emplace_back(fail); }

  inline const std::vector<SubFail> &subtest_fails() const { return subtest_fails_; }

 private:
  AssertLine line_{};
  std::vector<SubFail> subtest_fails_{};
};

class test_skipped final : public private_detail::test_exception {
  using test_exception::test_exception;
};

inline void skip(std::string reason) { throw test_skipped{std::move(reason)}; }

using test_fn = void();

class InternalTest {
 public:
  InternalTest(test_fn &fn, const Id &id, const Location &loc) noexcept
      : fn_{fn}, id_{id}, location_{loc} {}

  inline void run() const noexcept(false) { return fn_(); }

  inline Id id() const noexcept { return id_; }
  inline Location loc() const noexcept { return location_; }

  // TODO: Make these private functions only for friends
  inline InternalTest const *next() const noexcept { return next_; }
  inline void set_next(InternalTest const *next) noexcept { next_ = next; }

 private:
  test_fn &fn_;
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

RunResult run_internal(const InternalTest &test, Logger &log) noexcept;

void register_internal_test(InternalTest &entry) noexcept;

InternalTest const *internal_tests() noexcept;

}  // namespace private_detail

template <class>
class Registration : public InternalTest {
 public:
  Registration(test_fn &fn, const Id &id, const Location &location) noexcept
      : InternalTest{fn, id, location} {
    ::pt::private_detail::register_internal_test(*this);
  }
};

#define PT_OPERATOR =

namespace private_detail {

template <class Begin, class End>
struct SubTestRunner {
  pt::Location location;
  char const *var_name;
  Begin begin;
  End end;

  template <class Fn>
  auto operator PT_OPERATOR(const Fn &fn) noexcept(false) {
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
  }
};

}  // namespace private_detail

struct {
  template <class Begin, class End>
  constexpr auto from_iterators(const Location &location, const char *name, Begin begin,
                                End end) const noexcept {
    return private_detail::SubTestRunner<Begin, End>{location, name, begin, end};
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
    return this->operator()({}, "#UNK", iterable);
  }

  template <class T>
  constexpr auto operator()(const std::initializer_list<T> &iterable) const noexcept(false) {
    return this->operator()({}, "#UNK", iterable);
  }

} constexpr subtest;

struct SectionRunner {
  pt::Location location;
  char const *name{};

  template <class Fn>
  auto operator PT_OPERATOR(const Fn &fn) const noexcept(false) {
    try {
      fn();
    } catch (test_failed &fail) {
      fail.push_sub_fail(SubFail{location, "SECTION", name, ""});
      throw;
    }
  }
};

constexpr SectionRunner section(const Location &location, char const *name) noexcept {
  return SectionRunner{location, name};
}

constexpr SectionRunner section(char const *name) noexcept { return SectionRunner{{}, name}; }

struct nothrows {
  constexpr explicit nothrows(const AssertLine &line) noexcept : line_{line} {}

  template <class Fn>
  auto operator PT_OPERATOR(const Fn &fn) noexcept(false) {
    try {
      fn();
      return;
    } catch (const private_detail::test_exception &) {
      throw;
    } catch (...) {
      throw test_failed{"NOTHROWS FAILED", line_};
    }

    assert(false);
  }

 private:
  AssertLine line_{};
};

template <class Exception>
class throws {
 public:
  constexpr throws() noexcept = default;

  constexpr explicit throws(const AssertLine &line) noexcept : line_{line} {}

  template <class Fn>
  auto operator PT_OPERATOR(const Fn &fn) noexcept(false) {
    try {
      fn();
      throw test_failed{"THROWS FAILED", line_};
    } catch (const Exception &) {
      return;
    } catch (const private_detail::test_exception &) {
      throw;
    } catch (...) {
      throw test_failed{"WRONG EXCEPTION THROWN", line_};
    }

    assert(false);
  }

 private:
  AssertLine line_{};
};

struct AssertionRunner {
  template <class Fn>
  auto operator PT_OPERATOR(const Fn &fn) const noexcept(false) {
    if (!fn()) {
      throw test_failed{"ASSERT FAILED", line_};
    }
  }

  AssertLine line_{};
};

constexpr AssertionRunner assertion(const AssertLine &line) noexcept { return {line}; }
constexpr AssertionRunner assertion() noexcept { return {}; }

}  // namespace pt

#define PT_SUITE(name_space)                \
  namespace {                               \
  struct {                                  \
    char const *namespace_str{#name_space}; \
    size_t line{__LINE__};                  \
  } static constexpr pt_test_suite;         \
  }                                         \
  namespace

#define PT_TEST(name)                                                                 \
  struct name : ::pt::Registration<name> {                                            \
    using ::pt::Registration<name>::Registration;                                     \
    static void run() noexcept(false);                                                \
  } const static name##_registration{                                                 \
      name::run, {{pt_test_suite.namespace_str}, {#name}}, {{__FILE__}, {__LINE__}}}; \
                                                                                      \
  void name::run() noexcept(false)

#define PT_ASSERTION_LINE_W_EXPRESSION(expression) \
  ::pt::AssertLine { {{__FILE__}, {__LINE__}}, #expression }

#define PT_ASSERTION_LINE \
  ::pt::AssertLine { {{__FILE__}, {__LINE__}}, nullptr }

#define PT_ASSUME(expression) assert(expression)

#define PT_ASSERT(expression) \
  ::pt::assertion(PT_ASSERTION_LINE_W_EXPRESSION(expression)) = [&]() { return (expression); }

#define PT_THROWS(exception, expression)                                         \
  ::pt::throws<exception>(PT_ASSERTION_LINE_W_EXPRESSION(#expression)) = [&]() { \
    (void)(expression);                                                          \
  };

#define PT_NOTHROWS(expression) \
  ::pt::nothrows(PT_ASSERTION_LINE_W_EXPRESSION(#expression)) = [&]() { (void)(expression); }

#define PT_LOCATION          \
  ::pt::Location {           \
    {__FILE__}, { __LINE__ } \
  }

#define PT_SUBTEST(var, ...) \
  ::pt::subtest(PT_LOCATION, #var, __VA_ARGS__) = [&](const auto &var) noexcept(false)

#define PT_SECTION(name) ::pt::section(PT_LOCATION, #name) = [&]() noexcept(false)

#endif  // __cplusplus

#endif  // PPOSIX_PT_HPP
