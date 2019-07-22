#pragma once

#include <netinet/in.h>


namespace pposix::socket {

enum class protocol : int {
    ip = IPPROTO_IP,
    icmp = IPPROTO_ICMP,
    igmp = IPPROTO_IGMP,
    tcp = IPPROTO_TCP,
    pup = IPPROTO_PUP,
    udp = IPPROTO_UDP,
    idp = IPPROTO_IDP,
    raw = IPPROTO_RAW,
};

}
