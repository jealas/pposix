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
  inline InternalTest const *spawn_tests() const noexcept { return spawn_tests_; }
  inline InternalTest const *unknown_tests() const noexcept { return unknown_tests_; }

  inline void register_internal_test(TestType type, InternalTest &entry) noexcept {
    assert(entry.next() == nullptr);

    const auto tail{[=]() -> InternalTest const ** {
      switch (type) {
        case TestType::Normal:
          return &normal_tests_;
        case TestType::Spawn:
          return &spawn_tests_;
        default:
          return &unknown_tests_;
      }
    }()};

    // Only and last modification to the registration entry
    entry.set_next(*tail);
    *tail = &entry;
  }

 private:
  InternalTest const *normal_tests_{nullptr};
  InternalTest const *spawn_tests_{nullptr};
  InternalTest const *unknown_tests_{nullptr};

} static registrar{};

void register_internal_test(TestType type, InternalTest &entry) noexcept {
  registrar.register_internal_test(type, entry);
}

}  // namespace private_detail
}  // namespace pt

namespace pt {
namespace capi {

extern "C" {

PtTestEntry pt_normal_tests() noexcept {
  return PtTestEntry{static_cast<void const *>(private_detail::registrar.normal_tests())};
}

PtTestEntry pt_spawn_tests() noexcept {
  return PtTestEntry{static_cast<void const *>(private_detail::registrar.spawn_tests())};
}

PtTestEntry pt_unknown_tests() noexcept {
  return PtTestEntry{static_cast<void const *>(private_detail::registrar.unknown_tests())};
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

PtTestType pt_test_entry_type(const PtTestEntry entry) noexcept {
  const auto test{static_cast<InternalTest const *>(entry.handle)};
  return PtTestType{pt_test_type(static_cast<pt_test_type_t>(test->type()))};
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
    pt_spawn_tests,
    pt_test_entries_stop,
    pt_test_entries_next,
    pt_test_entry_type,
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

namespace private_detail {

InternalTest const *internal_tests() noexcept { return registrar.normal_tests(); }

RunResult run_internal(const InternalTest &test) noexcept {
  try {
    std::cout << "Running " << test.id() << std::endl;
    test.run();
    return RunResult::Success;

  } catch (const test_skipped &skipped) {
    return RunResult::Skipped;
  } catch (const test_failed &fail) {
    std::cerr << "FAILED: " << test.loc() << '[' << test.id() << ']' << '\n'
              << '\t' << fail.what() << '\n';
    std::cerr << std::endl;
    return RunResult::Failed;

  } catch (const std::runtime_error &error) {
    std::cerr << "ERROR: Uncaught runtime error while running test " << test.id() << '\n'
              << error.what();
    std::cerr << std::endl;

    return RunResult::Error;

  } catch (const std::exception &exception) {
    std::cerr << "ERROR: Uncaught exception while running test " << test.id() << '\n'
              << exception.what();
    std::cerr << std::endl;

    return RunResult::Exception;
  } catch (...) {
    std::cerr << "INTERNAL ERROR: Unknown exception caught while running test " << test.id();
    std::cerr << std::endl;

    return RunResult::InternalError;
  }
}

}  // namespace private_detail

}  // namespace pt
