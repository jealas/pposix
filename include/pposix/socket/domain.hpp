#ifndef PPOSIX_SOCKET_DOMAIN_HPP
#define PPOSIX_SOCKET_DOMAIN_HPP

#include <sys/socket.h>


namespace pposix::socket {

    enum class domain : unsigned {
        local = AF_LOCAL,
        inet4 = AF_INET,
        inet6 = AF_INET6,
        bluetooth = AF_BLUETOOTH
    };

}

#endif //PPOSIX_SOCKET_DOMAIN_HPP
