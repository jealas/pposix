#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <type_traits>

namespace pposix {

class char_cspan {
 public:
  constexpr char_cspan() noexcept = default;

  constexpr char_cspan(const char_cspan &) noexcept = default;
  constexpr char_cspan(char_cspan &&) noexcept = default;

  constexpr char_cspan &operator=(const char_cspan &) noexcept = default;
  constexpr char_cspan &operator=(char_cspan &&) noexcept = default;

  constexpr char_cspan(char const *buffer, std::size_t length) noexcept
      : buffer_{buffer}, length_{buffer ? length : 0u} {}

  template <std::size_t Length>
  constexpr char_cspan(char (&buffer)[Length]) noexcept  // NOLINT implicit constructor
      : buffer_{buffer}, length_{Length} {}

  template <std::size_t Length>
  constexpr char_cspan(  // NOLINT implicit constructor
      std::array<char, Length> &buffer) noexcept
      : buffer_{buffer.data()}, length_{Length} {}

  constexpr bool empty() const noexcept { return length_ == 0u; }

  constexpr char const *data() const noexcept { return buffer_; }

  constexpr std::size_t length() const noexcept { return length_; }

  constexpr char const *begin() const noexcept { return buffer_; }
  constexpr char const *end() const noexcept { return buffer_ + length_; }

  constexpr char const *cbegin() const noexcept { return buffer_; }
  constexpr char const *cend() const noexcept { return buffer_ + length_; }

  constexpr char_cspan subspan(std::size_t offset, std::size_t count) const noexcept {
    if (offset >= length_ or (length_ - offset) < count) {
      return {};
    } else {
      return {buffer_ + offset, count};
    }
  }

 private:
  char const *buffer_{nullptr};
  std::size_t length_{};
};

class char_span {
 public:
  constexpr char_span() noexcept = default;

  constexpr char_span(const char_span &) noexcept = default;
  constexpr char_span(char_span &&) noexcept = default;

  constexpr char_span &operator=(const char_span &) noexcept = default;
  constexpr char_span &operator=(char_span &&) noexcept = default;

  constexpr char_span(char *buffer, std::size_t length) noexcept
      : buffer_{buffer}, length_{buffer ? length : 0u} {}

  template <std::size_t Length>
  constexpr char_span(char (&buffer)[Length]) noexcept  // NOLINT implicit constructor
      : buffer_{buffer}, length_{Length} {}

  template <std::size_t Length>
  constexpr char_span(  // NOLINT implicit constructor
      std::array<char, Length> &buffer) noexcept
      : buffer_{buffer.data()}, length_{Length} {}

  constexpr bool empty() const noexcept { return length_ == 0u; }

  constexpr char *data() noexcept { return buffer_; }
  constexpr char const *data() const noexcept { return buffer_; }

  constexpr std::size_t length() const noexcept { return length_; }

  constexpr char *begin() noexcept { return buffer_; }
  constexpr char *end() noexcept { return buffer_ + length_; }

  constexpr char const *cbegin() const noexcept { return buffer_; }
  constexpr char const *cend() const noexcept { return buffer_ + length_; }

  constexpr operator char_cspan() const noexcept {  // NOLINT implicit converter
    return char_cspan{data(), length()};
  }

  constexpr char_span subspan(std::size_t offset, std::size_t count) const noexcept {
    if (offset >= length_ or (length_ - offset) < count) {
      return {};
    } else {
      return {buffer_ + offset, count};
    }
  }

 private:
  char *buffer_{nullptr};
  std::size_t length_{};
};

}  // namespace pposix
