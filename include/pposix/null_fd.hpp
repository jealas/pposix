#pragma once

namespace pposix {

struct nullfd_t {
  constexpr bool operator==(nullfd_t) const noexcept { return true; }
};

constexpr nullfd_t nullfd{};

}  // namespace pposix
