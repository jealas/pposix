#include "pt.hpp"

#include <cassert>
#include <future>

#include "pposix/dlfcn.hpp"
#include "pposix/util.hpp"

namespace pt {

struct {
  InternalTest *tail{nullptr};
} static registrar{};

namespace private_detail {

void register_internal_test(InternalTest &entry) noexcept {
  assert(entry.next() == nullptr);

  // Only and last modification to the registration entry
  entry.set_next(registrar.tail);

  registrar.tail = &entry;
}

InternalTest const *internal_tests() noexcept { return registrar.tail; }

}  // namespace private_detail

void InternalTest::run() const {
  throw std::logic_error{"Default InternalTest::run should never be called"};
}

struct TestResult {};

struct TestCase {
  virtual ~TestCase() = default;

  TestCase() = default;

  TestCase(const TestCase &) = delete;
  TestCase(TestCase &&) = delete;

  TestCase &operator=(const TestCase &) = delete;
  TestCase &operator=(TestCase &&) = delete;

  inline virtual const Test &test() const {
    throw std::logic_error{"pt::Test::test should never be called."};
  };

  virtual std::future<TestResult> run() const {
    throw std::logic_error{"pt::Test::run should never be called."};
  }
};

class NormalTest final : public TestCase {
 public:
  inline explicit NormalTest(const Test &test) : test_{test} {}

  inline const Test &test() const override { return test_; }

  inline std::future<TestResult> run() const override {
    std::promise<TestResult> p{};
    p.set_value(TestResult{});
    return p.get_future();
  }

 private:
  const Test &test_;
};

class ThreadTest final : public TestCase {
 public:
  inline explicit ThreadTest(const Test &test) : test_{test} {}

  inline const Test &test() const override { return test_; }

  inline std::future<TestResult> run() const override {
    std::promise<TestResult> p{};
    p.set_value(TestResult{});
    return p.get_future();
  }

 private:
  const Test &test_;
};

class SpawnTest final : public TestCase {
 public:
  inline explicit SpawnTest(const Test &test) : test_{test} {}

  inline const Test &test() const override { return test_; }

  inline std::future<TestResult> run() const override {
    std::promise<TestResult> p{};
    p.set_value(TestResult{});
    return p.get_future();
  }

 private:
  const Test &test_;
};

std::unique_ptr<TestCase> wrap_test(const Test &test) {
  switch (test.type()) {
    case Type::Normal:
      return std::make_unique<NormalTest>(test);
    case Type::Thread:
      return std::make_unique<ThreadTest>(test);
    case Type::Spawn:
      return std::make_unique<SpawnTest>(test);
    default:
      throw std::logic_error{"Unhandled test type: " +
                             std::to_string(pposix::underlying_v(test.type()))};
  }
}

void run(const Test &test) noexcept {
  try {
    std::cout << "Running " << test.id() << std::endl;
    test.run();
    return;

  } catch (const test_failed &fail) {
    std::cerr << "FAILED: " << test.loc() << '[' << test.id() << ']' << '\n'
              << '\t' << fail.what() << '\n';
    std::cerr << std::endl;
    return;

  } catch (const std::runtime_error &error) {
    std::cerr << "ERROR: Uncaught runtime error while running test " << test.id() << '\n'
              << error.what();
    std::cerr << std::endl;
    std::exit(EXIT_FAILURE);
  } catch (const std::exception &exception) {
    std::cerr << "ERROR: Uncaught exception while running test " << test.id() << '\n'
              << exception.what();
    std::cerr << std::endl;
    return;

  } catch (...) {
    std::cerr << "INTERNAL ERROR: Unknown exception caught while running test " << test.id();
    std::cerr << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

namespace capi {

extern "C" {

Entry pt_test_entries() noexcept { return Entry{static_cast<void *>(registrar.tail)}; }

Stop pt_test_entries_stop(const Entry entry) noexcept { return {entry.handle == nullptr}; }

Entry pt_test_entries_next(const Entry entry) noexcept {
  if (entry.handle == nullptr) {
    return Entry{nullptr};
  } else {
    const auto test{static_cast<InternalTest const *>(entry.handle)};
    return Entry{static_cast<void const *>(test->next())};
  }
}

Type pt_test_entry_type(const Entry entry) noexcept {
  const auto test{static_cast<InternalTest const *>(entry.handle)};
  return Type{pt_test_type{pposix::underlying_v(test->type())}};
}

Namespace pt_test_entry_namespace(const Entry entry) noexcept {
  const auto test{static_cast<InternalTest const *>(entry.handle)};
  return {test->id().name_space};
}

Name pt_test_entry_name(const Entry entry) noexcept {
  const auto test{static_cast<InternalTest const *>(entry.handle)};
  return {test->id().name};
}

File pt_test_entry_file(const Entry entry) noexcept {
  const auto test{static_cast<InternalTest const *>(entry.handle)};
  return {test->loc().file};
}

Line pt_test_entry_line(const Entry entry) noexcept {
  const auto test{static_cast<InternalTest const *>(entry.handle)};
  return {test->loc().line};
}

RunResult pt_test_entry_run(const Entry entry) noexcept {
  if (entry.handle == nullptr) {
    return {pt_run_result::run_error};
  }

  try {
    const auto test{static_cast<InternalTest const *>(entry.handle)};

    pt::run(*test);

    return {pt_run_result::run_success};
  } catch (...) {
    return {pt_run_result::run_internal_error};
  }
}

symbol_table pt_symbol_table{
    pt_test_entries,    pt_test_entries_stop,    pt_test_entries_next,
    pt_test_entry_type, pt_test_entry_namespace, pt_test_entry_name,
    pt_test_entry_file, pt_test_entry_line,      pt_test_entry_run,
};
}

}  // namespace capi

}  // namespace pt
