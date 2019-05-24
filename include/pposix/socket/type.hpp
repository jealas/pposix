#ifndef PPOSIX_SOCKET_TYPE_HPP
#define PPOSIX_SOCKET_TYPE_HPP

#include <sys/socket.h>


namespace pposix::socket {

    enum class type : unsigned {
        stream = SOCK_STREAM,
        dgram = SOCK_DGRAM,
        seq = SOCK_SEQPACKET,
        raw = SOCK_RAW,
        rdm = SOCK_RDM
    };

}

#endif //PPOSIX_SOCKET_TYPE_HPP
