#ifndef PPOSIX_FILE_CLOSE_HPP
#define PPOSIX_FILE_CLOSE_HPP

#include <system_error>

#include <unistd.h>

#include "pposix/file/rawfd.hpp"
#include "pposix/util/underlying_value.hpp"
#include "pposix/errno_code.hpp"


namespace pposix::file {

    std::error_code close(rawfd fd) {
        const auto error = ::close(util::underlying_value(fd));

        if (error == -1) {
            return errno_code();
        }
        else {
            return {};
        }
    }

}

#endif //PPOSIX_FILE_CLOSE_HPP
