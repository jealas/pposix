#ifndef PPOSIX_SOCKET_MAKE_SOCKET_HPP
#define PPOSIX_SOCKET_MAKE_SOCKET_HPP


#include "posix/result.hpp"
#include "posix/rawfd.hpp"
#include "posix/nullfd.hpp"
#include "util/underlying_value.hpp"
#include "posix/errno_code.hpp"

#include "domain.hpp"
#include "type.hpp"
#include "protocol.hpp"
#include "flag.hpp"
#include "fd.hpp"


namespace pposix::socket {

    posix::result<fd> make_socket(const domain domain, const type typ, const flag &flags = flag::none, const protocol protocol = 0) {
        const posix::rawfd fd{::socket(util::underlying_value(domain), util::underlying_value(typ) | util::underlying_value(flags), protocol)};

        if (fd == posix::nullfd) {
            return posix::errno_code();
        }
        else {
            return socket_fd{fd};
        }
    }

}

#endif //PPOSIX_SOCKET_MAKE_SOCKET_HPP
