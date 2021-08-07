#include "pt.hpp"

namespace pt {
namespace private_detail {

class GlobalRegistrar {
 public:
  inline InternalTest const *normal_tests() const noexcept { return normal_tests_; }

  inline void register_internal_test(InternalTest &entry) noexcept {
    assert(entry.next() == nullptr);

    // Only and last modification to the registration entry
    entry.set_next(normal_tests_);
    normal_tests_ = &entry;
  }

 private:
  InternalTest const *normal_tests_{nullptr};

} static registrar{};

void register_internal_test(InternalTest &entry) noexcept {
  registrar.register_internal_test(entry);
}

}  // namespace private_detail
}  // namespace pt

namespace pt {
namespace capi {

extern "C" {

PtTestEntry pt_normal_tests() noexcept {
  return PtTestEntry{static_cast<void const *>(private_detail::registrar.normal_tests())};
}

PtTestEntriesStop pt_test_entries_stop(const PtTestEntry entry) noexcept {
  return {entry.handle == nullptr};
}

PtTestEntry pt_test_entries_next(const PtTestEntry entry) noexcept {
  if (entry.handle == nullptr) {
    return PtTestEntry{nullptr};
  } else {
    const auto test{static_cast<InternalTest const *>(entry.handle)};
    return PtTestEntry{static_cast<void const *>(test->next())};
  }
}

PtTestNamespace pt_test_entry_namespace(const PtTestEntry entry) noexcept {
  const auto test{static_cast<InternalTest const *>(entry.handle)};
  return {test->id().name_space};
}

PtTestName pt_test_entry_name(const PtTestEntry entry) noexcept {
  const auto test{static_cast<InternalTest const *>(entry.handle)};
  return {test->id().name};
}

PtTestFile pt_test_entry_file(const PtTestEntry entry) noexcept {
  const auto test{static_cast<InternalTest const *>(entry.handle)};
  return {test->loc().file};
}

PtTestLine pt_test_entry_line(const PtTestEntry entry) noexcept {
  const auto test{static_cast<InternalTest const *>(entry.handle)};
  return {test->loc().line};
}

PtTestRunResult pt_test_entry_run(const PtTestEntry entry, const PtLogger log) noexcept {
  if (entry.handle == nullptr) {
    return {pt_run_result::run_error};
  }

  try {
    const auto test{static_cast<InternalTest const *>(entry.handle)};
    auto logger{Logger{log}};
    return {static_cast<pt_run_result_t>(pt::private_detail::run_internal(*test, logger))};
  } catch (...) {
    return {pt_run_result::run_internal_error};
  }
}

PtSymbolTable PT_SYMBOL_TABLE_NAME{
    {PT_CAPI_SECRET, PT_CAPI_VERSION},
    pt_normal_tests,
    pt_test_entries_stop,
    pt_test_entries_next,
    pt_test_entry_namespace,
    pt_test_entry_name,
    pt_test_entry_file,
    pt_test_entry_line,
    pt_test_entry_run,
};

}  // extern "C"

}  // namespace capi
}  // namespace pt

namespace pt {

namespace private_detail {

InternalTest const *internal_tests() noexcept { return registrar.normal_tests(); }

RunResult run_internal(const InternalTest &test, Logger &logger) noexcept {
  auto out_stream{logger.stream(LogLevel::Info)};
  auto err_stream{logger.stream(LogLevel::Error)};

  try {
    out_stream << "Running " << test.id() << '\n';

    test.run();

    return RunResult::Success;

  } catch (const pt::test_skipped &skipped) {
    return RunResult::Skipped;

  } catch (const pt::test_failed &fail) {
    err_stream << "FAILED: " << test.loc() << '[' << test.id() << ']' << '\n';

    const auto &subtest_fails{fail.subtest_fails()};

    const auto indent{"  "};
    std::string indent_level{indent};
    for (auto begin{subtest_fails.crbegin()}, end{subtest_fails.crend()}; begin < end; ++begin) {
      err_stream << indent_level << begin->label << ": ";

      if (!begin->message.empty()) {
        err_stream << begin->message << " @ ";
      }
      err_stream << begin->location << '[' << begin->name << ']' << '\n';

      indent_level += indent;
    }

    indent_level += indent;
    err_stream << indent_level << fail.message() << ": " << fail.line() << '\n';
    err_stream << '\n';

    return RunResult::Failed;

  } catch (const pt::internal_error &error) {
    err_stream << "INTERNAL ERROR: Internal error occurred while running test " << test.id()
               << '\n'
               << error.what() << '\n';

    return RunResult::InternalError;

  } catch (const std::runtime_error &error) {
    err_stream << "ERROR: Uncaught runtime error while running test " << test.id() << '\n'
               << error.what() << '\n';

    return RunResult::Error;

  } catch (const std::exception &exception) {
    err_stream << "ERROR: Uncaught exception while running test " << test.id() << '\n'
               << exception.what() << '\n';

    return RunResult::Error;

  } catch (...) {
    err_stream << "EXCEPTION: Unknown exception caught while running test " << test.id();
    err_stream << '\n';

    return RunResult::Exception;
  }
}

}  // namespace private_detail

}  // namespace pt
