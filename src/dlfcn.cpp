#include "pposix/dlfcn.hpp"

#include <iostream>

#include "pposix/errno.hpp"

namespace pposix::capi {

std::error_code dlclose(const raw_symtable s) noexcept {
  if (const auto res{::dlclose(static_cast<raw_symtable_t>(s))}; res != 0) {
    return std::make_error_code(std::errc::bad_address);
  } else {
    return {};
  }
}

result<symtable_d> dlopen(const char *filename) noexcept {
  if (const auto res{::dlopen(filename, RTLD_GLOBAL | RTLD_LAZY)}; res == nullptr) {
    return std::make_error_code(std::errc::bad_address);
  } else {
    return symtable_d{raw_symtable{res}};
  }
}

result<sym_d> dlsym(const raw_symtable s, const char *name) noexcept {
  if (const auto res{::dlsym(static_cast<raw_symtable_t>(s), name)}; res == nullptr) {
    return std::make_error_code(std::errc::bad_address);
  } else {
    return sym_d{raw_sym{res}};
  }
}

}  // namespace pposix::capi