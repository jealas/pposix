#ifndef PPOSIX_BUFFER_SPAN_HPP
#define PPOSIX_BUFFER_SPAN_HPP

#include <algorithm>
#include <array>
#include <cstddef>
#include <type_traits>

namespace pposix {

class buffer_span {
 public:
  ~buffer_span() = default;
  constexpr buffer_span() = default;

  constexpr buffer_span(const buffer_span &) = default;
  constexpr buffer_span(buffer_span &&) = default;

  constexpr buffer_span &operator=(const buffer_span &) = default;
  constexpr buffer_span &operator=(buffer_span &&) = default;

  constexpr buffer_span(std::byte *buffer, std::size_t length) noexcept
      : buffer_{buffer}, length_{buffer ? length : 0u} {}

  template <std::size_t Length>
  constexpr /*implicit*/ buffer_span(std::byte (&buffer)[Length]) noexcept
      : buffer_{buffer}, length_{Length} {}

  template <std::size_t Length>
  constexpr /*implicit*/ buffer_span(std::array<std::byte, Length> &buffer) noexcept
      : buffer_{buffer.data()}, length_{Length} {}

  constexpr bool empty() const noexcept { return length_ == 0u; }

  constexpr std::byte *data() noexcept { return buffer_; }
  constexpr std::byte const *data() const noexcept { return buffer_; }

  constexpr std::size_t length() const noexcept { return length_; }

  constexpr std::byte *begin() noexcept { return buffer_; }
  constexpr std::byte *end() noexcept { return buffer_ + length_; }

  constexpr std::byte *cbegin() const noexcept { return buffer_; }
  constexpr std::byte *cend() const noexcept { return buffer_ + length_; }

  constexpr buffer_span subspan(std::size_t offset, std::size_t count) const noexcept {
    if (offset >= length_ or (length_ - offset) < count) {
      return {};
    } else {
      return {buffer_ + offset, count};
    }
  }

 private:
  std::byte *buffer_{nullptr};
  std::size_t length_{0u};
};

}  // namespace pposix

#endif  // PPOSIX_BUFFER_SPAN_HPP
