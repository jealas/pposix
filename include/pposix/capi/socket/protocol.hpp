#pragma once

#include <netinet/in.h>


namespace pposix::capi::socket {

enum class protocol : int {
    icmp = IPPROTO_ICMP
};

}
