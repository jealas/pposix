#pragma once

#include <system_error>
#include <type_traits>
#include <utility>
#include <variant>

namespace pposix {

struct bad_result_access : std::runtime_error {
  using std::runtime_error::runtime_error;
};

template <class T>
class result;

namespace detail {

template <class T>
const std::error_code *result_get_error_unsafe(const result<T> &) noexcept;

template <class T>
const T *result_get_value_unsafe(const result<T> &) noexcept;

template <class T>
T *result_get_value_unsafe(result<T> &) noexcept;

}  // namespace detail

template <class T>
class result {
  static_assert(not std::is_same_v<std::decay_t<T>, std::error_code>,
                "The type of the result cannot be a std::error_code because it already holds an "
                "internal std::error_code.");

  struct get_error_visitor {
    std::error_code operator()(const std::error_code &ec) const noexcept { return ec; }
    std::error_code operator()(const T &) const noexcept(false) {
      throw bad_result_access{"pposix::result error was accessed when it contained a value."};
    }
  };

  struct get_immutable_value_visitor {
    const T &operator()(const std::error_code &) const noexcept(false) {
      throw bad_result_access{"pposix::result value was accessed when it contained an error."};
    }
    const T &operator()(const T &value) const noexcept { return value; }
  };

  struct get_mutable_value_visitor {
    T &operator()(const std::error_code &) const noexcept(false) {
      throw bad_result_access{"pposix::result value was accessed when it contained an error."};
    }
    T &operator()(T &value) const noexcept { return value; }
  };

  struct is_error_visitor {
    bool operator()(const std::error_code &) const noexcept { return true; }
    bool operator()(const T &) const noexcept { return false; }
  };

 public:
  result(std::error_code ec) noexcept : result_{ec} {}                 // NOLINT implicit constructor
  constexpr result(const T &value) noexcept : result_{value} {}        // NOLINT implicit constructor
  constexpr result(T &&value) noexcept : result_{std::move(value)} {}  // NOLINT implicit constructor

  [[nodiscard]] std::error_code error() const noexcept(false) {
    return std::visit(get_error_visitor{}, result_);
  }

  [[nodiscard]] T &value() noexcept(false) {
    return std::visit(get_mutable_value_visitor{}, result_);
  }
  [[nodiscard]] T const &value() const noexcept(false) {
    return std::visit(get_immutable_value_visitor{}, result_);
  }

  bool has_error() const noexcept { return std::visit(is_error_visitor{}, result_); }
  bool has_value() const noexcept { return not has_error(); }

  explicit operator bool() const noexcept { return has_value(); }

  const T &operator*() const noexcept { return *detail::result_get_value_unsafe(*this); }
  T &operator*() noexcept { return *detail::result_get_value_unsafe(*this); }

  const T *operator->() const noexcept { return *detail::result_get_value_unsafe(*this); }
  T *operator->() noexcept { return *detail::result_get_value_unsafe(*this); }

  // Friends
  template <class U>
  friend const std::error_code *pposix::detail::result_get_error_unsafe(
      const result<U> &) noexcept;

  template <class U>
  friend const U *pposix::detail::result_get_value_unsafe(const result<U> &) noexcept;

  template <class U>
  friend U *pposix::detail::result_get_value_unsafe(result<U> &) noexcept;

 private:
  std::variant<T, std::error_code> result_;
};

namespace detail {

template <class T>
const std::error_code *result_get_error_unsafe(const result<T> &res) noexcept {
  return std::get_if<std::error_code>(&res.result_);
}

template <class T>
const T *result_get_value_unsafe(const result<T> &res) noexcept {
  return std::get_if<T>(&res.result_);
}

template <class T>
T *result_get_value_unsafe(result<T> &res) noexcept {
  return std::get_if<T>(&res.result_);
}

}  // namespace detail

template <class U, class T, class Func>
result<U> result_map(result<T> result, Func func) noexcept {
  static_assert(noexcept(func(std::declval<T&&>())));

  if (auto * const value_ptr = detail::result_get_value_unsafe(result)) {
    return func(std::move(*value_ptr));
  } else {
    return *detail::result_get_error_unsafe(result);
  }
}

}  // namespace pposix
