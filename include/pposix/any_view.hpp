#pragma once

#include <array>
#include <cstddef>
#include <memory>

#include "pposix/byte_span.hpp"

namespace pposix {

class any_cview {
 public:
  constexpr any_cview() noexcept = default;

  constexpr any_cview(void const *data, std::size_t len) noexcept : data_{data}, len_{len} {}

  template <class T, size_t Count>
  explicit constexpr any_cview(std::array<const T, Count> *array) noexcept
      : data_{array->data()}, len_{sizeof(T) * array->size()} {}

  template <class T, size_t Count>
  explicit constexpr any_cview(const T (*array)[Count]) noexcept
      : data_{array}, len_{sizeof(T) * Count} {}

  template <class T>
  explicit constexpr any_cview(const T *ptr) noexcept : data_{ptr}, len_{sizeof(T)} {}

  constexpr any_cview(const any_cview &) noexcept = default;
  constexpr any_cview(any_cview &&) noexcept = default;

  constexpr any_cview &operator=(const any_cview &) noexcept = default;
  constexpr any_cview &operator=(any_cview &&) noexcept = default;

  constexpr bool empty() const noexcept { return length() == 0u; }

  constexpr void const *data() const noexcept { return data_; }

  constexpr std::size_t length() const noexcept { return len_; }

  constexpr byte_cspan as_bytes() const noexcept {
    return {static_cast<std::byte const *>(data_), len_};
  }

 private:
  void const *data_{nullptr};
  std::size_t len_{};
};

class any_view {
 public:
  constexpr any_view() noexcept = default;

  constexpr any_view(void *data, std::size_t len) noexcept : data_{data}, len_{len} {}

  template <class T, size_t Count>
  explicit constexpr any_view(std::array<const T, Count> *array) noexcept
      : data_{array->data()}, len_{sizeof(T) * array->size()} {}

  template <class T, size_t Count>
  explicit constexpr any_view(T (*array)[Count]) noexcept
      : data_{array}, len_{sizeof(T) * Count} {}

  template <class T>
  explicit constexpr any_view(T *ptr) noexcept : data_{ptr}, len_{sizeof(T)} {}

  constexpr any_view(const any_view &) noexcept = default;
  constexpr any_view(any_view &&) noexcept = default;

  constexpr any_view &operator=(const any_view &) noexcept = default;
  constexpr any_view &operator=(any_view &&) noexcept = default;

  constexpr bool empty() const noexcept { return length() == 0u; }

  constexpr void *data() noexcept { return data_; }
  constexpr void const *data() const noexcept { return data_; }

  constexpr std::size_t length() const noexcept { return len_; }

  constexpr operator any_cview() const noexcept {  // NOLINT implicit converter
    return {data(), length()};
  }

  constexpr byte_cspan as_bytes() const noexcept {
    return {static_cast<std::byte const *>(data_), len_};
  }

  constexpr byte_span as_writeable_bytes() const noexcept {
    return {static_cast<std::byte *>(data_), len_};
  }

 private:
  void *data_{nullptr};
  std::size_t len_{};
};

}