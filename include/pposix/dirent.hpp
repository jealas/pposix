#pragma once

#include <string_view>
#include <system_error>

#include <dirent.h>

#include "pposix/result.hpp"
#include "pposix/unique_d.hpp"
#include "pposix/unique_fd.hpp"

namespace pposix {

struct dirent_close_policy {
  std::error_code operator()(DIR *dir) const noexcept;
};

class dirent;

class dir_fd {
 public:
  dir_fd() = default;

  explicit dir_fd(raw_fd fd) noexcept;

  dir_fd(const dir_fd &) = delete;
  dir_fd(dir_fd &&) = default;

  dir_fd &operator=(const dir_fd &) = delete;
  dir_fd &operator=(dir_fd &&) = default;

  raw_fd get() const noexcept;
  [[nodiscard]] raw_fd release() noexcept;

 private:
  unique_fd dir_fd_{};
};

class dirent {
 public:
  dirent() = default;

  explicit dirent(DIR *dir) noexcept;

  DIR *get() noexcept;
  [[nodiscard]] DIR *release() noexcept;

  static result<dirent> opendir(char const *dirname) noexcept;

 private:
  struct tag {};
  unique_d<tag, DIR *, std::integral_constant<DIR *, nullptr>, dirent_close_policy> dirent_d_{};
};

result<dirent> fdopendir(dir_fd fd) noexcept {
  const auto dirfd = fd.get();

  if (DIR *dir = ::fdopendir(dirfd); dir == nullptr) {
    return current_errno_code();
  } else {
    (void)fd.release();
    return dirent{dir};
  }
}

}  // namespace pposix
