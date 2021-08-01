#pragma once

#include "pposix/fd.hpp"

namespace pposix {

class directory {
 public:
  ~directory() = default;
  directory() = default;

  explicit directory(dir_fd fd) noexcept;

  directory(const directory &) = delete;
  directory(directory &&) = default;

  directory &operator=(const directory &) = delete;
  directory &operator=(directory &&) = default;

  std::error_code close() noexcept;

 private:
  dir_fd fd_{};
};

}  // namespace pposix