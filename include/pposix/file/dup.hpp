#ifndef PPOSIX_FILE_DUP_HPP
#define PPOSIX_FILE_DUP_HPP

#include <unistd.h>

#include "pposix/file/fd.hpp"
#include "pposix/result.hpp"
#include "pposix/file/rawfd.hpp"
#include "pposix/util/underlying_value.hpp"
#include "pposix/file/nullfd.hpp"
#include "pposix/errno_code.hpp"


namespace pposix::file {

    template <class ClosePolicy>
    result<fd<>> dup(const fd<ClosePolicy> &fd) {
        const rawfd newfd{::dup(util::underlying_value(fd.raw()))};

        if (newfd == nullfd) {
            return errno_code();
        }
        else {
            return file::fd<>{newfd};
        }
    }

}

#endif //PPOSIX_FILE_DUP_HPP
