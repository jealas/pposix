#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <type_traits>

namespace pposix {

class byte_cspan {
 public:
  byte_cspan() = default;

  constexpr byte_cspan(const byte_cspan &) = default;
  constexpr byte_cspan(byte_cspan &&) = default;

  constexpr byte_cspan &operator=(const byte_cspan &) = default;
  constexpr byte_cspan &operator=(byte_cspan &&) = default;

  constexpr byte_cspan(std::byte const *buffer, std::size_t length) noexcept
      : buffer_{buffer}, length_{buffer ? length : 0u} {}

  template <std::size_t Length>
  constexpr /*implicit*/ byte_cspan(std::byte (&buffer)[Length]) noexcept
      : buffer_{buffer}, length_{Length} {}

  template <std::size_t Length>
  constexpr /*implicit*/ byte_cspan(std::array<std::byte, Length> &buffer) noexcept
      : buffer_{buffer.data()}, length_{Length} {}

  constexpr bool empty() const noexcept { return length_ == 0u; }

  constexpr std::byte const *data() const noexcept { return buffer_; }

  constexpr std::size_t length() const noexcept { return length_; }

  constexpr std::byte const *begin() const noexcept { return buffer_; }
  constexpr std::byte const *end() const noexcept { return buffer_ + length_; }

  constexpr std::byte const *cbegin() const noexcept { return buffer_; }
  constexpr std::byte const *cend() const noexcept { return buffer_ + length_; }

  constexpr byte_cspan subspan(std::size_t offset, std::size_t count) const noexcept {
    if (offset >= length_ or (length_ - offset) < count) {
      return {};
    } else {
      return {buffer_ + offset, count};
    }
  }

 private:
  std::byte const *buffer_;
  std::size_t length_;
};

class byte_span {
 public:
  byte_span() = default;

  constexpr byte_span(const byte_span &) = default;
  constexpr byte_span(byte_span &&) = default;

  constexpr byte_span &operator=(const byte_span &) = default;
  constexpr byte_span &operator=(byte_span &&) = default;

  constexpr byte_span(std::byte *buffer, std::size_t length) noexcept
      : buffer_{buffer}, length_{buffer ? length : 0u} {}

  template <std::size_t Length>
  constexpr /*implicit*/ byte_span(std::byte (&buffer)[Length]) noexcept
      : buffer_{buffer}, length_{Length} {}

  template <std::size_t Length>
  constexpr /*implicit*/ byte_span(std::array<std::byte, Length> &buffer) noexcept
      : buffer_{buffer.data()}, length_{Length} {}

  constexpr bool empty() const noexcept { return length_ == 0u; }

  constexpr std::byte *data() noexcept { return buffer_; }
  constexpr std::byte const *data() const noexcept { return buffer_; }

  constexpr std::size_t length() const noexcept { return length_; }

  constexpr std::byte *begin() noexcept { return buffer_; }
  constexpr std::byte *end() noexcept { return buffer_ + length_; }

  constexpr std::byte const *cbegin() const noexcept { return buffer_; }
  constexpr std::byte const *cend() const noexcept { return buffer_ + length_; }

  constexpr operator byte_cspan() const noexcept { return byte_cspan{data(), length()}; }

  constexpr byte_span subspan(std::size_t offset, std::size_t count) const noexcept {
    if (offset >= length_ or (length_ - offset) < count) {
      return {};
    } else {
      return {buffer_ + offset, count};
    }
  }

 private:
  std::byte *buffer_;
  std::size_t length_;
};

}  // namespace pposix
