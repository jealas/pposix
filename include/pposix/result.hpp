#pragma once

#include <system_error>
#include <type_traits>
#include <variant>

namespace pposix {

template <class T>
class result {
  static_assert(std::is_trivial_v<T>);

  struct error_visitor {
    std::error_code operator()(const std::error_code &ec) const noexcept { return ec; }
    std::error_code operator()(const T &) const noexcept { return {}; }
  };

 public:
  result(std::error_code ec) : result_{ec} {}  // NOLINT implicit constructor

  constexpr result(const T &value) : result_{value} {}  // NOLINT implicit constructor

  std::error_code error() const noexcept { return std::visit(error_visitor{}, result_); }

  T *value() noexcept { return std::get_if<T>(&result_); }

  T const *value() const noexcept { return std::get_if<T>(&result_); }

 private:
  std::variant<T, std::error_code> result_;
};

}  // namespace pposix
