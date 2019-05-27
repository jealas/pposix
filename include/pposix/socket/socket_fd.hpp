#ifndef PPOSIX_SOCKET_SOCKET_HPP
#define PPOSIX_SOCKET_SOCKET_HPP

#include <sys/socket.h>

#include "posix/fd.hpp"


namespace pposix::socket {

    template <class Close>
    struct socket_fd : public posix::fd<Close> {
        using posix::fd<Close>::fd;
    };

}

#endif //PPOSIX_SOCKET_SOCKET_HPP
