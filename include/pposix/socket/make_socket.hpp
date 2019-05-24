#ifndef PPOSIX_SOCKET_MAKE_SOCKET_HPP
#define PPOSIX_SOCKET_MAKE_SOCKET_HPP

#include <sys/socket.h>

#include "posix/result.hpp"
#include "posix/rawfd.hpp"
#include "posix/nullfd.hpp"
#include "util/underlying_value.hpp"
#include "posix/errno_code.hpp"
#include "socket/domain.hpp"
#include "socket/type.hpp"
#include "socket/protocol.hpp"
#include "socket/flag.hpp"


namespace pposix::socket {

    posix::result<socket> make_socket(const domain domain, const type typ, const flag &flags = flag::none, const protocol protocol = 0) {
        const posix::rawfd fd{::socket(util::underlying_value(domain), util::underlying_value(typ) | util::underlying_value(flags), protocol)};

        if (fd == posix::nullfd) {
            return posix::errno_code();
        }
        else {
            return socket{fd};
        }
    }

}

#endif //PPOSIX_SOCKET_MAKE_SOCKET_HPP
