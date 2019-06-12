#pragma once

#include <string>
#include <system_error>
#include <type_traits>
#include <utility>

namespace pposix {

template <class T, class Deleter>
class resource {
  static_assert(std::is_trivial_v<Deleter>);

 public:
  ~resource() noexcept(false) {
    const auto error = reset();
    if (error) {
      throw std::system_error{error,
                              "Failed to automatically close resource. Try manually calling "
                              "reset() to catch and handle the error"};
    }
  }

  constexpr resource() = default;
  constexpr explicit resource(nullptr_t) : resource::resource{} {};
  constexpr explicit resource(T *resource) : resource_{resource} {};

  constexpr resource(T *resource, const Deleter &deleter)
      : resource_{resource}, deleter_{deleter} {};

  constexpr resource(T *resource, Deleter &&deleter)
      : resource_{resource}, deleter_{std::move(deleter)} {};

  constexpr T *get() noexcept { return resource_; }
  constexpr T const *get() const noexcept { return resource_; }

  constexpr T &operator*() noexcept { return *resource_; }
  constexpr const T &operator*() const noexcept { return *resource_; }

  constexpr T &operator->() noexcept { return *resource_; }
  constexpr const T &operator->() const noexcept { return *resource_; }

  constexpr T *release() noexcept {
    T *old_resource = resource_;
    resource_ = nullptr;

    return old_resource;
  }

  std::error_code reset(T *resource = nullptr) noexcept {
    const auto error = deleter_(resource_);
    if (error) {
      return error;
    } else {
      resource_ = resource;
      return {};
    }
  }

  constexpr bool empty() const noexcept { return resource_ == nullptr; }
  constexpr explicit operator bool() const noexcept { return not empty(); }

 private:
  T *resource_{nullptr};
  Deleter deleter_{};
};

}  // namespace pposix
