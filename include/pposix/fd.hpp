#ifndef PPOSIX_FD_HPP
#define PPOSIX_FD_HPP

#include <utility>
#include <type_traits>

#include "default_close.hpp"
#include "nullfd.hpp"
#include "rawfd.hpp"
#include "errno_code.hpp"


namespace pposix {

    template<class Close = default_close>
    class [[nodiscard]] fd {
    public:
        constexpr fd() noexcept : raw_fd_{nullfd} {}

        constexpr /*implicit*/ fd(nullfd_t) noexcept : raw_fd_{} {}

        constexpr explicit fd(const rawfd file_descriptor) : raw_fd_{file_descriptor} {}

        ~fd() {
            if (const auto error = close()) {
                // TODO: Log this fatal error.
            }
        }

        fd(const fd &other) = delete;

        fd(fd &&other) noexcept : raw_fd_{other.raw_fd_} {
            other.raw_fd_ = nullfd;
        }

        fd &operator=(const fd &) = delete;

        fd &operator=(fd &&) = delete;

        bool empty() const noexcept { return raw_fd_ == nullfd; }

        explicit operator bool() const noexcept { return not empty(); }

        rawfd raw() const noexcept { return raw_fd_; }

        [[nodiscard]]
        rawfd release() noexcept {
            const auto tmp_fd = raw_fd_;
            raw_fd_ = nullfd;
            return tmp_fd;
        }

        [[nodiscard]]
        std::error_code close() noexcept {
            if (not empty()) {
                if (const auto error = Close{}(raw())) {
                    return errno_code();
                }
            }

            raw_fd_ = nullfd;

            return {};
        }

    private:
        rawfd raw_fd_{};
    };
}

#endif //PPOSIX_FD_HPP
