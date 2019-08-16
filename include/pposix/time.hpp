#pragma once

#include <time.h>

#include "pposix/duration.hpp"

namespace pposix {

class timespec : public ::timespec {
 public:
  inline timespec() noexcept = default;

  inline timespec(seconds s) noexcept : ::timespec{} {
    this->tv_sec = s.count();
    this->tv_nsec = 0;
  }

  inline timespec(seconds s, nanoseconds ns) noexcept : ::timespec{} {
    this->tv_sec = s.count();
    this->tv_nsec = ns.count();
  }
};

}