#ifndef PPOSIX_MAKE_FD_HPP
#define PPOSIX_MAKE_FD_HPP

#include "fd.hpp"
#include "is_fd.hpp"

namespace pposix {

    template <class Fd, class ... FuncArgs, class ... Args>
    boost::outcome_v2::std_result<Fd> make_fd(rawfd_t(*fd_factory)(FuncArgs...), Args && ... args) {
        static_assert(is_fd_v<Fd>);

        const auto fd = fd_factory(std::forward<Args>(args)...);
        if (fd == nullfd) {
            return std::error_code{errno, std::system_category()};
        }
        else {
            return Fd{fd};
        }
    }

}

#endif //PPOSIX_MAKE_FD_HPP
