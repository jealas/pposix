#ifndef PPOSIX_SOCKET_FLAG_HPP
#define PPOSIX_SOCKET_FLAG_HPP

#include <sys/socket.h>


namespace pposix::socket {

    enum class flag : unsigned {
        none = 0u,
        closexec = SOCK_CLOEXEC,
        nonblock = SOCK_NONBLOCK
    };

    flag operator | (const flag &lhs, const flag &rhs) noexcept {
        return flag{util::underlying_value(lhs) | util::underlying_value(rhs)};
    }
}

#endif //PPOSIX_SOCKET_FLAG_HPP
