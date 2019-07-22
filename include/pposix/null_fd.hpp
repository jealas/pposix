#pragma once

namespace pposix {

struct null_fd_t {
  constexpr bool operator==(null_fd_t) const noexcept { return true; }
};

constexpr null_fd_t nullfd{};

}  // namespace pposix
