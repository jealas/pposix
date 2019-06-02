#ifndef PPOSIX_BUFFER_SPAN_HPP
#define PPOSIX_BUFFER_SPAN_HPP

#include <array>
#include <cstddef>
#include <type_traits>


namespace pposix {

    class buffer_span {

      static_assert(std::is_trivial_v<buffer_span>);

    public:
        buffer_span(std::byte *buffer, std::size_t length) noexcept
        : buffer_{buffer}, length_{length} {}

        template <std::size_t Length>
        buffer_span(const std::byte (&buffer)[Length]) noexcept
            : buffer_{buffer}, length_{Length} {}

        std::byte *data() noexcept { return buffer_; }
        std::byte const *data() const noexcept { return buffer_; }

        std::size_t length() const noexcept { return length_; }

    private:
        std::byte *buffer_{};
        std::size_t length_{};
    };

}

#endif //PPOSIX_BUFFER_SPAN_HPP
