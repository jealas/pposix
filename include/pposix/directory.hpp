#pragma once

#include "pposix/dirent.hpp"
#include "pposix/fd.hpp"

namespace pposix {

class directory {
 public:
  class entry {
   public:
    ~entry() = default;
    inline explicit entry(capi::dir_descriptor dir) : dir_{std::move(dir)} {}

    entry(const entry &) noexcept = delete;
    entry(entry &&) noexcept = default;

    entry &operator=(const entry &) noexcept = delete;
    entry &operator=(entry &&) noexcept = default;

   private:
    capi::dir_descriptor dir_{};
  };

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