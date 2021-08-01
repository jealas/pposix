#pragma once

#include <sys/stat.h>
#include <unistd.h>

#include <string_view>
#include <system_error>

#include "pposix/byte_span.hpp"
#include "pposix/fcntl.hpp"
#include "pposix/fd.hpp"
#include "pposix/platform.hpp"
#include "pposix/result.hpp"
#include "pposix/stat.hpp"
#include "pposix/util.hpp"

namespace pposix {

enum class file_seek { set = SEEK_SET, current = SEEK_CUR, end = SEEK_END };

class file {
 public:
  file() = default;

  explicit file(fd fd) noexcept;

  file(const file &) = delete;
  file(file &&) = default;

  file &operator=(const file &) = delete;
  file &operator=(file &&) = default;

  std::error_code close() noexcept;

  result<off_t> lseek(off_t offset, file_seek wh) noexcept;

  result<ssize_t> read(byte_span buffer) noexcept;
  result<ssize_t> write(byte_cspan buffer) noexcept;

 private:
  fd fd_{};
};

}  // namespace pposix
