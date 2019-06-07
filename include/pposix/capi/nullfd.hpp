#pragma once

#include <type_traits>

#include "pposix/capi/rawfd.hpp"
#include "pposix/util/underlying_value.hpp"

namespace pposix::capi {

struct nullfd_t {
  constexpr bool operator==(nullfd_t) const noexcept { return true; }
  constexpr operator rawfd() const noexcept { return rawfd{-1}; }
};

constexpr nullfd_t nullfd{};

constexpr bool operator==(rawfd fd, nullfd_t) noexcept { return fd.fd() == fd_t{-1}; }
constexpr bool operator==(nullfd_t, rawfd fd) noexcept { return fd == nullfd; }

constexpr bool operator!=(rawfd fd, nullfd_t) noexcept { return !(fd == nullfd); }
constexpr bool operator!=(nullfd_t, rawfd fd) noexcept { return fd != nullfd; }

}  // namespace pposix::capi
