#include "pt.hpp"

// Include the iostream header first
#include <iostream>
// DO NOT MOVE

#include <cassert>

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

PtTestRunResult pt_test_entry_run(const PtTestEntry entry) noexcept {
  if (entry.handle == nullptr) {
    return {pt_run_result::run_error};
  }

  try {
    const auto test{static_cast<InternalTest const *>(entry.handle)};
    return {static_cast<pt_run_result_t>(pt::private_detail::run_internal(*test))};
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

inline std::ostream &operator<<(std::ostream &out, const Id &id) {
  return out << id.name_space.val << "::" << id.name.val;
}

inline std::ostream &operator<<(std::ostream &out, const Location &location) {
  return out << location.file.val << ':' << location.line.val;
}

inline std::ostream &operator<<(std::ostream &out, const AssertLine &line) {
  if (line.expression) {
    out << line.expression << (line.location.file.val ? " @ " : "");
  }

  if (line.location.file.val) {
    out << line.location.file.val << ':' << line.location.line.val;
  }

  return out;
}

namespace private_detail {

InternalTest const *internal_tests() noexcept { return registrar.normal_tests(); }

RunResult run_internal(const InternalTest &test) noexcept {
  try {
    std::cout << "Running " << test.id() << std::endl;
    test.run();

    return RunResult::Success;

  } catch (const pt::test_skipped &skipped) {
    return RunResult::Skipped;

  } catch (const pt::test_failed &fail) {
    std::cerr << "FAILED: " << test.loc() << '[' << test.id() << ']' << '\n';

    const auto &subtest_fails{fail.subtest_fails()};

    const auto indent{"  "};
    std::string indent_level{indent};
    for (auto begin{subtest_fails.crbegin()}, end{subtest_fails.crend()}; begin < end; ++begin) {
      std::cerr << indent_level << begin->label << ": ";

      if (!begin->message.empty()) {
        std::cerr << begin->message << " @ ";
      }
      std::cerr << begin->location << '[' << begin->name << ']' << '\n';

      indent_level += indent;
    }

    indent_level += indent;
    std::cerr << indent_level << fail.message() << ": " << fail.line() << '\n';
    std::cerr << std::endl;

    return RunResult::Failed;

  } catch (const pt::internal_error &error) {
    std::cerr << "INTERNAL ERROR: Internal error occurred while running test " << test.id() << '\n'
              << error.what();
    std::cerr << std::endl;

    return RunResult::InternalError;

  } catch (const std::runtime_error &error) {
    std::cerr << "ERROR: Uncaught runtime error while running test " << test.id() << '\n'
              << error.what();
    std::cerr << std::endl;

    return RunResult::Error;

  } catch (const std::exception &exception) {
    std::cerr << "ERROR: Uncaught exception while running test " << test.id() << '\n'
              << exception.what();
    std::cerr << std::endl;

    return RunResult::Error;

  } catch (...) {
    std::cerr << "EXCEPTION: Unknown exception caught while running test " << test.id();
    std::cerr << std::endl;

    return RunResult::Exception;
  }
}

}  // namespace private_detail

}  // namespace pt
