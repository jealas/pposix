#ifndef PPOSIX_FILE_NULLFD_HPP
#define PPOSIX_FILE_NULLFD_HPP

#include "pposix/file/rawfd.hpp"


namespace pposix::file {

    struct nullfd_t {
        constexpr /*implicit*/ operator rawfd() const noexcept { return rawfd::null; }
    };

    static constexpr nullfd_t nullfd{};
}

#endif //PPOSIX_FILE_NULLFD_HPP
