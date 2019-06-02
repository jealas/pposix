#ifndef PPOSIX_WRITE_HPP
#define PPOSIX_WRITE_HPP

#include <cstddef>

#include <unistd.h>

#include "pposix/result.hpp"
#include "pposix/buffer_span.hpp"
#include "pposix/file/fd.hpp"


namespace pposix::file {

    template <class ClosePolicy>
    result<buffer_span> write(const file::fd<ClosePolicy> &fd, const buffer_span buffer) noexcept {
    }

}


#endif //PPOSIX_WRITE_HPP
