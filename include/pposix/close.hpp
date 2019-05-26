#ifndef PPOSIX_CLOSE_HPP
#define PPOSIX_CLOSE_HPP

#include <system_error>

#include <unistd.h>

#include "rawfd.hpp"
#include "errno_code.hpp"
#include "util/underlying_value.hpp"


namespace pposix {

    struct close {
        std::error_code operator()(rawfd fd) const noexcept {
            if (const auto error = ::close(util::underlying_value(fd)); error == -1) {
                return errno_code();
            }

            return {};
        }
    };

}

#endif //PPOSIX_CLOSE_HPP
