#include "pposix/sysconf.hpp"

#include "pposix/errno.hpp"
#include "pposix/util/underlying_value.hpp"

namespace pposix {

result<system_config_value> sysconf(system_config_name config_name) noexcept {
  errno_context errno_ctx;

  if (const auto value = ::sysconf(util::underlying_value(config_name)); value != -1) {
    return value;
  } else if (const auto error = current_errno_code()) {
    return error;
  } else {
    return value;
  }
}

}