#pragma once

#include <cstddef>
#include <memory>

#include "pposix/byte_span.hpp"

namespace pposix {

class any_cspan {
 public:
  constexpr any_cspan() noexcept = default;

  constexpr any_cspan(void const *data, std::size_t len) noexcept : data_{data}, len_{len} {}

  template <class T>
  explicit constexpr any_cspan(T &value) noexcept
      : data_{std::addressof(value)}, len_{sizeof(T)} {}

  constexpr any_cspan(const any_cspan &) noexcept = default;
  constexpr any_cspan(any_cspan &&) noexcept = default;

  constexpr any_cspan &operator=(const any_cspan &) noexcept = default;
  constexpr any_cspan &operator=(any_cspan &&) noexcept = default;

  constexpr void const *data() const noexcept { return data_; }

  constexpr std::size_t length() const noexcept { return len_; }

  constexpr byte_cspan as_bytes() const noexcept {
    return {static_cast<std::byte const *>(data_), len_};
  }

 private:
  void const *data_{nullptr};
  std::size_t len_{};
};

class any_span {
 public:
  constexpr any_span() noexcept = default;

  constexpr any_span(void *data, std::size_t len) noexcept : data_{data}, len_{len} {}

  template <class T>
  explicit constexpr any_span(T &value) noexcept : data_{std::addressof(value)}, len_{sizeof(T)} {}

  constexpr any_span(const any_span &) noexcept = default;
  constexpr any_span(any_span &&) noexcept = default;

  constexpr any_span &operator=(const any_span &) noexcept = default;
  constexpr any_span &operator=(any_span &&) noexcept = default;

  constexpr void *data() noexcept { return data_; }
  constexpr void const *data() const noexcept { return data_; }

  constexpr std::size_t length() const noexcept { return len_; }

  constexpr operator any_cspan() const noexcept {  // NOLINT implicit converter
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