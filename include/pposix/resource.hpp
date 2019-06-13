#pragma once

#include <string>
#include <system_error>
#include <type_traits>
#include <utility>

namespace pposix {

template <class T, class ClosePolicy>
class [[nodiscard]] resource {
  static_assert(std::is_trivial_v<ClosePolicy>);

 public:
  ~resource() noexcept(false) {
    if (const auto error = close()) {
      throw std::system_error{error,
                              "Failed to automatically close resource. Try manually calling "
                              "close() to catch and handle the error"};
    }
  }

  constexpr resource() = default;
  constexpr explicit resource(nullptr_t) : resource::resource{} {};
  constexpr explicit resource(T * resource) : resource_{resource} {};

  constexpr resource(T * resource, const ClosePolicy &close)
      : resource_{resource}, close_{close} {};

  constexpr resource(T * resource, ClosePolicy && close)
      : resource_{resource}, close_{std::move(close)} {};

  constexpr T *get() noexcept { return resource_; }
  constexpr T const *get() const noexcept { return resource_; }

  constexpr ClosePolicy &get_close() noexcept { return close_; }
  constexpr const ClosePolicy &get_close() const noexcept { return close_; }

  constexpr T &operator*() noexcept { return *resource_; }
  constexpr const T &operator*() const noexcept { return *resource_; }

  constexpr T &operator->() noexcept { return *resource_; }
  constexpr const T &operator->() const noexcept { return *resource_; }

  [[nodiscard]] constexpr T *release() noexcept {
    T *old_resource = resource_;
    resource_ = nullptr;

    return old_resource;
  }

  [[nodiscard]] std::error_code close() noexcept {
    if (empty()) {
      return {};
    }

    while (const auto ec = close_(resource_)) {
      if (ec == std::errc::interrupted) {
        continue
      } else {
        return ec;
      }
    }

    resource_ = nullptr;

    return {};
  }

  constexpr bool empty() const noexcept { return resource_ == nullptr; }
  constexpr explicit operator bool() const noexcept { return not empty(); }

 private:
  T *resource_{nullptr};
  ClosePolicy close_{};
};

}  // namespace pposix
