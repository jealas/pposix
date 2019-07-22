#pragma once

#include <string_view>

#include "pposix/capi/file/permission.hpp"

namespace pposix::capi::file {

    enum class permission : unsigned {
        none = 0u,

        user_read = S_IRUSR,
        user_write = S_IWUSR,
        user_execute = S_IXUSR,
        user_all = S_IRWXU,

        group_read = S_IRGRP,
        group_write = S_IWGRP,
        group_execute = S_IXGRP,
        group_all = S_IRWXG,

        other_read = S_IROTH,
        other_write = S_IWOTH,
        other_execute = S_IXOTH,
        other_all = S_IRWXO,
    };

    constexpr permission operator|(permission lhs, permission rhs) noexcept {
        return permission{util::underlying_value(lhs) | util::underlying_value(rhs)};
    }

    constexpr permission &operator|=(permission &lhs, permission rhs) noexcept {
        lhs = lhs | rhs;
        return lhs;
    }


}  // namespace pposix::capi::file

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
