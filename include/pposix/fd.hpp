#ifndef PPOSIX_FD_HPP
#define PPOSIX_FD_HPP

#include <utility>
#include <type_traits>

#include "close.hpp"
#include "nullfd.hpp"
#include "rawfd.hpp"
#include "errno_code.hpp"


namespace pposix {

    template<class Close = close>
    class [[nodiscard]] fd {
    public:
        constexpr fd() noexcept : fd::fd{nullfd} {}

        constexpr /*implicit*/ fd(nullfd_t) noexcept : raw_fd_{nullfd}, close_{} {}

        constexpr explicit fd(const rawfd file_descriptor)
                : raw_fd_{file_descriptor}, close_{} {}

        constexpr explicit fd(const rawfd file_descriptor, const Close &close)
            : raw_fd_{file_descriptor}, close_{close} {}

        constexpr explicit fd(const rawfd file_descriptor, Close &&close)
            : raw_fd_{file_descriptor}, close_{std::move(close)} {}

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
                while (const auto ec = close_(raw())) {
                    if (ec == std::errc::interrupted) {
                        continue;
                    } else {
                        return ec;
                    }
                }

                raw_fd_ = nullfd;
            }

            return {};
        }

    private:
        rawfd raw_fd_;
        Close close_;
    };
}

#endif //PPOSIX_FD_HPP
