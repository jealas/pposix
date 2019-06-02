#ifndef PPOSIX_FILE_FD_HPP
#define PPOSIX_FILE_FD_HPP

#include <utility>
#include <type_traits>

#include "pposix/file/default_close_policy.hpp"
#include "pposix/file/nullfd.hpp"
#include "pposix/file/rawfd.hpp"
#include "pposix/errno_code.hpp"


namespace pposix::file {

    template<class ClosePolicy = default_close_policy>
    class [[nodiscard]] fd {
    public:
        constexpr fd() noexcept : fd::fd{nullfd} {}

        constexpr /*implicit*/ fd(nullfd_t) noexcept : raw_fd_{nullfd}, close_{} {}

        constexpr explicit fd(const rawfd file_descriptor)
                : raw_fd_{file_descriptor}, close_{} {}

        constexpr explicit fd(const rawfd file_descriptor, const ClosePolicy &close)
            : raw_fd_{file_descriptor}, close_{close} {}

        constexpr explicit fd(const rawfd file_descriptor, ClosePolicy &&close)
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

        ClosePolicy & get_close_policy() noexcept { return close_; }

        const ClosePolicy & get_close_policy() const noexcept { return close_; }

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
        ClosePolicy close_;
    };
}

#endif //PPOSIX_FILE_FD_HPP
