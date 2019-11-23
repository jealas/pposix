#include "pposix/syslog.hpp"

#include <syslog.h>

namespace pposix {

void openlog(const char *identifier, syslog_option options, syslog_facility facility) noexcept {
  ::openlog(identifier, underlying_v(options), underlying_v(facility));
}

void openlog(const std::string &identifier, syslog_option options,
             syslog_facility facility) noexcept {
  pposix::openlog(identifier.c_str(), options, facility);
}

syslog_priority_set setlogmask(syslog_priority_set priorities) noexcept {
  return syslog_priority_set{::setlogmask(priorities.to_mask())};
}

void closelog() noexcept { ::closelog(); }

}  // namespace pposix
