#ifndef PPOSIX_SOCKET_SOCKET_HPP
#define PPOSIX_SOCKET_SOCKET_HPP

#include <sys/socket.h>

#include "posix/fd.hpp"


namespace pposix::socket {

    struct fd : public posix::fd<> {
        using posix::fd<>::fd;
    };

}

#endif //PPOSIX_SOCKET_SOCKET_HPP
