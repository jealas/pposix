#pragma once

#include <array>

namespace pposix {

template <class T>
class cspan {
 public:
  constexpr cspan() noexcept = default;

  constexpr cspan(const cspan &) noexcept = default;
  constexpr cspan(cspan &&) noexcept = default;

  constexpr cspan &operator=(const cspan &) noexcept = default;
  constexpr cspan &operator=(cspan &&) noexcept = default;

  constexpr cspan(T const *buffer, std::size_t length) noexcept
      : buffer_{buffer}, length_{buffer ? length : 0u} {}

  template <std::size_t Length>
  constexpr cspan(const T (&buffer)[Length]) noexcept  // NOLINT implicit constructor
      : buffer_{buffer}, length_{Length} {}

  template <std::size_t Length>
  constexpr cspan(  // NOLINT implicit constructor
      const std::array<T, Length> &buffer) noexcept
      : buffer_{buffer.data()}, length_{Length} {}

  constexpr bool empty() const noexcept { return length_ == 0u; }

  constexpr const T *data() const noexcept { return buffer_; }

  constexpr std::size_t length() const noexcept { return length_; }

  constexpr const T *begin() const noexcept { return buffer_; }
  constexpr const T *end() const noexcept { return buffer_ + length_; }

  constexpr const T *cbegin() const noexcept { return buffer_; }
  constexpr const T *cend() const noexcept { return buffer_ + length_; }

  constexpr cspan subspan(std::size_t offset, std::size_t count) const noexcept {
    if (offset >= length_ or (length_ - offset) < count) {
      return {};
    } else {
      return {buffer_ + offset, count};
    }
  }

 private:
  T const *buffer_{nullptr};
  std::size_t length_{};
};

template <class T>
class span {
 public:
  constexpr span() noexcept = default;

  constexpr span(const span &) noexcept = default;
  constexpr span(span &&) noexcept = default;

  constexpr span &operator=(const span &) noexcept = default;
  constexpr span &operator=(span &&) noexcept = default;

  constexpr span(T *buffer, std::size_t length) noexcept
      : buffer_{buffer}, length_{buffer ? length : 0u} {}

  template <std::size_t Length>
  constexpr span(T (&buffer)[Length]) noexcept  // NOLINT implicit constructor
      : buffer_{buffer}, length_{Length} {}

  template <std::size_t Length>
  constexpr span(  // NOLINT implicit constructor
      std::array<T, Length> &buffer) noexcept
      : buffer_{buffer.data()}, length_{Length} {}

  constexpr bool empty() const noexcept { return length_ == 0u; }

  constexpr T *data() noexcept { return buffer_; }
  constexpr T const *data() const noexcept { return buffer_; }

  constexpr std::size_t length() const noexcept { return length_; }

  constexpr T *begin() noexcept { return buffer_; }
  constexpr T *end() noexcept { return buffer_ + length_; }

  constexpr T const *cbegin() const noexcept { return buffer_; }
  constexpr T const *cend() const noexcept { return buffer_ + length_; }

  constexpr operator cspan<T>() const noexcept {  // NOLINT implicit converter
    return cspan{data(), length()};
  }

  constexpr span subspan(std::size_t offset, std::size_t count) const noexcept {
    if (offset >= length_ or (length_ - offset) < count) {
      return {};
    } else {
      return {buffer_ + offset, count};
    }
  }

 private:
  T *buffer_{nullptr};
  std::size_t length_{};
};

}  // namespace pposix
