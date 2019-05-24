#ifndef PPOSIX_DEFAULT_CLOSE_HPP
#define PPOSIX_DEFAULT_CLOSE_HPP

#include <system_error>

#include <unistd.h>

#include "rawfd.hpp"
#include "errno_code.hpp"
#include "util/underlying_value.hpp"


namespace pposix {

    struct default_close {
        std::error_code operator()(rawfd fd) const noexcept {
            while (const auto error = ::close(util::underlying_value(fd))) {
                const auto ec = errno_code();

                if (ec == std::errc::interrupted) {
                    continue;
                } else {
                    return ec;
                }
            }

            return {};
        }
    };

}

#endif //PPOSIX_DEFAULT_CLOSE_HPP
