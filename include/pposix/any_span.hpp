#pragma once

#include <cstddef>
#include <memory>

#include "pposix/byte_span.hpp"

namespace pposix {

class any_cspan {
 public:
  any_cspan() = default;

  constexpr any_cspan(void const *data, std::size_t len) noexcept : data_{data}, len_{len} {}

  template <class T>
  explicit constexpr any_cspan(T &value) : data_{std::addressof(value)}, len_{sizeof(T)} {}

  any_cspan(const any_cspan &) = default;
  any_cspan(any_cspan &&) = default;

  any_cspan &operator=(const any_cspan &) = default;
  any_cspan &operator=(any_cspan &&) = default;

  constexpr void const *data() const noexcept { return data_; }

  constexpr std::size_t length() const noexcept { return len_; }

  byte_cspan as_bytes() const noexcept {
    return {reinterpret_cast<std::byte const *>(data_), len_};
  }

 private:
  void const *data_;
  std::size_t len_;
};

class any_span {
 public:
  any_span() = default;

  constexpr any_span(void *data, std::size_t len) noexcept : data_{data}, len_{len} {}

  template <class T>
  explicit constexpr any_span(T &value) : data_{std::addressof(value)}, len_{sizeof(T)} {}

  any_span(const any_span &) = default;
  any_span(any_span &&) = default;

  any_span &operator=(const any_span &) = default;
  any_span &operator=(any_span &&) = default;

  constexpr void *data() noexcept { return data_; }
  constexpr void const *data() const noexcept { return data_; }

  constexpr std::size_t length() const noexcept { return len_; }

  constexpr operator any_cspan() const noexcept {  // NOLINT implicit converter
    return {data(), length()};
  }

  byte_cspan as_bytes() const noexcept {
    return {reinterpret_cast<std::byte const *>(data_), len_};
  }

  byte_span as_writeable_bytes() const noexcept {
    return {reinterpret_cast<std::byte *>(data_), len_};
  }

 private:
  void *data_;
  std::size_t len_;
};

}