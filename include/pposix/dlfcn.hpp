#ifndef PPOSIX_DLFCN_HPP
#define PPOSIX_DLFCN_HPP

#include <dlfcn.h>

#include <string_view>

#include "pposix/descriptor.hpp"
#include "pposix/result.hpp"

namespace pposix::capi {

using raw_symtable_t = void *;

struct raw_symtable : raw_d<raw_symtable_t> {
  using raw_d<raw_symtable_t>::raw_d;

  constexpr bool operator==(const raw_symtable other) const noexcept {
    return static_cast<raw_symtable_t>(*this) == static_cast<raw_symtable_t>(other);
  }
};

std::error_code dlclose(raw_symtable) noexcept;

using symtable_d =
    descriptor<raw_symtable, descriptor_constant<raw_symtable, raw_symtable_t, nullptr>, dlclose>;

result<symtable_d> dlopen(const char *filename) noexcept;

using raw_sym_t = void *;

struct raw_sym : raw_d<raw_sym_t> {
  using raw_d<raw_sym_t>::raw_d;

  constexpr bool operator==(const raw_sym other) const noexcept {
    return static_cast<raw_sym_t>(*this) == static_cast<raw_sym_t>(other);
  }
};

inline std::error_code close_sym(raw_sym) noexcept { return {}; }

using sym_d = descriptor<raw_sym, descriptor_constant<raw_sym, raw_sym_t, nullptr>, close_sym>;

result<sym_d> dlsym(raw_symtable, const char *name) noexcept;

}  // namespace pposix::capi

namespace pposix {

template <class T>
class symbol_table {
  explicit symbol_table(capi::symtable_d symtable) : symtable_{std::move(symtable)} {}

  std::error_code load_lazy(const char *name) noexcept {
    const auto res{capi::dlsym(symtable_.raw(), name)};
    if (res.has_error()) {
      return res.error();
    }

    table = static_cast<T const *>(*res);

    if (table == nullptr) {
      return std::make_error_code(std::errc::argument_out_of_domain);
    } else {
      return {};
    }
  }

  T const &operator->() { return *table; }

  capi::symtable_d symtable_{};
  T const *table{};
};

}  // namespace pposix

#endif  // PPOSIX_DLFCN_HPP
