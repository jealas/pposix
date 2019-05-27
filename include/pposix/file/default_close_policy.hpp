#ifndef PPOSIX_DEFAULT_CLOSE_POLICY_HPP
#define PPOSIX_DEFAULT_CLOSE_POLICY_HPP

#include <system_error>

#include <unistd.h>

#include "pposix/errno_code.hpp"
#include "pposix/file/rawfd.hpp"
#include "pposix/util/underlying_value.hpp"


namespace pposix::file {

    struct default_close_policy {
        std::error_code operator()(rawfd fd) const noexcept {
            if (const auto error = ::close(util::underlying_value(fd)); error == -1) {
                return errno_code();
            }

            return {};
        }
    };

}

#endif //PPOSIX_DEFAULT_CLOSE_POLICY_HPP
