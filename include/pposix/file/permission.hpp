#pragma once

#include <string_view>

#include "pposix/capi/file/permission.hpp"

namespace pposix::file::literals::permission_literals {

namespace detail {

constexpr capi::file::permission str_to_permission(
    char const *const c_str, size_t len, capi::file::permission (*char_to_permission)(char)) {
  capi::file::permission p{};

  for (const auto c : std::string_view{c_str, len}) {
    p |= char_to_permission(c);
  }

  return p;
}

template <capi::file::permission Read, capi::file::permission Write,
          capi::file::permission Execute>
constexpr capi::file::permission char_to_permission(const char c) noexcept {
  switch (c) {
    case 'r':
      return Read;
    case 'w':
      return Write;
    case 'x':
      return Execute;

    default:
      break;
  }

  return capi::file::permission::none;
}
}  // namespace detail

constexpr capi::file::permission operator"" _user(char const *c_str, size_t len) {
  return detail::str_to_permission(
      c_str, len,
      detail::char_to_permission<capi::file::permission::user_read,
                                 capi::file::permission::user_write,
                                 capi::file::permission::user_execute>);
}

constexpr capi::file::permission operator"" _group(char const *c_str, size_t len) {
  return detail::str_to_permission(
      c_str, len,
      detail::char_to_permission<capi::file::permission::group_read,
                                 capi::file::permission::group_write,
                                 capi::file::permission::group_execute>);
}

constexpr capi::file::permission operator"" _other(char const *c_str, size_t len) {
  return detail::str_to_permission(
      c_str, len,
      detail::char_to_permission<capi::file::permission::other_read,
                                 capi::file::permission::other_write,
                                 capi::file::permission::other_execute>);
}

}  // namespace pposix::file::literals::permission_literals
