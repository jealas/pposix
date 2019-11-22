#pragma once

#include <algorithm>
#include <initializer_list>
#include <string>

#include <syslog.h>

#include "pposix/util.hpp"

namespace pposix {

// Forward declarations
class syslog_priority_set;
syslog_priority_set setlogmask(syslog_priority_set priorities) noexcept;

enum class syslog_priority : int {
  emergency = LOG_EMERG,
  alert = LOG_ALERT,
  critical = LOG_CRIT,
  error = LOG_ERR,
  warning = LOG_WARNING,
  notice = LOG_NOTICE,
  info = LOG_INFO,
  debug = LOG_DEBUG
};

class syslog_priority_set {
 public:
  constexpr syslog_priority_set() noexcept = default;

  constexpr syslog_priority_set(std::initializer_list<syslog_priority> priorities) noexcept {
    for (auto priority : priorities) {
      set(priority);
    }
  }

  constexpr void set(syslog_priority priority) noexcept {
    priority_mask_ |= LOG_MASK(underlying_value(priority));
  }

  constexpr bool test(syslog_priority priority) const noexcept {
    return priority_mask_ & LOG_MASK(underlying_value(priority));
  }

  friend syslog_priority_set pposix::setlogmask(syslog_priority_set priorities) noexcept;

 private:
  constexpr syslog_priority_set(int mask) noexcept : priority_mask_{mask} {}
  constexpr int to_mask() const noexcept { return priority_mask_; }

  int priority_mask_{};
};

enum class syslog_facility : int {
  kernel = LOG_KERN,
  user = LOG_USER,
  mail = LOG_MAIL,
  news = LOG_NEWS,
  uucp = LOG_UUCP,
  daemon = LOG_DAEMON,
  auth = LOG_AUTH,
  cron = LOG_CRON,
  printer = LOG_LPR,
  local0 = LOG_LOCAL0,
  local1 = LOG_LOCAL1,
  local2 = LOG_LOCAL2,
  local3 = LOG_LOCAL3,
  local4 = LOG_LOCAL4,
  local5 = LOG_LOCAL5,
  local6 = LOG_LOCAL6,
  local7 = LOG_LOCAL7
};

enum class syslog_option : int {
  log_pid = LOG_PID,
  log_to_console = LOG_CONS,
  open_immediately = LOG_NDELAY,
  open_on_first_log = LOG_ODELAY,
  no_wait = LOG_NOWAIT
};

constexpr syslog_option operator|(syslog_option lhs, syslog_option rhs) noexcept {
  return syslog_option{underlying_value(lhs) | underlying_value(rhs)};
}

void openlog(const char *identifier, syslog_option options, syslog_facility facility) noexcept;
void openlog(const std::string &identifier, syslog_option options,
             syslog_facility facility) noexcept;

syslog_priority_set setlogmask(syslog_priority_set priorities) noexcept;

void closelog() noexcept;

template <class... Args>
void syslog(syslog_priority priority, const char *format, Args &&... args) noexcept {
  pposix::syslog(priority, syslog_facility::user, format, std::forward<Args>(args)...);
}

template <class... Args>
void syslog(syslog_priority priority, syslog_facility facility, const char *format,
            Args &&... args) noexcept {
  ::syslog(underlying_value(priority) | underlying_value(facility), format,
           std::forward<Args>(args)...);
}

}  // namespace pposix
