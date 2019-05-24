#ifndef PPOSIX_NULLFD_HPP
#define PPOSIX_NULLFD_HPP

#include "rawfd.hpp"


namespace pposix {

    struct nullfd_t {
        constexpr /*implicit*/ operator rawfd() const noexcept { return rawfd::null; }
    };

    static constexpr nullfd_t nullfd{};
}

#endif //PPOSIX_NULLFD_HPP
