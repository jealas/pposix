#ifndef PSERVICES_ERRNO_CODE_HPP
#define PSERVICES_ERRNO_CODE_HPP

#include <system_error>

#include <unistd.h>


namespace pposix {

    std::error_code errno_code() {
        return {errno, std::system_category()};
    }

}

#endif //PSERVICES_ERRNO_CODE_HPP
