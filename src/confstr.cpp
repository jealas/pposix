#include "pposix/confstr.hpp"

#include "pposix/errno.hpp"
#include "pposix/util.hpp"

namespace pposix {

result<size_t> confstr_length(confstr_name name) noexcept {
  errno_context context{};

  const auto length{::confstr(underlying_v(name), nullptr, 0u)};

  if (const auto error{current_errno_code()}) {
    return error;
  } else {
    return length;
  }
}

result<char_span> confstr(confstr_name name, char_span name_buffer) noexcept {
  errno_context context{};

  const auto length{::confstr(underlying_v(name), name_buffer.data(), name_buffer.length())};

  if (const auto error{current_errno_code()}) {
    return error;
  } else {
    return char_span{name_buffer.data(), length};
  }
}

}  // namespace pposix
