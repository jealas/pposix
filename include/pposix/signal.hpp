#pragma once

#include <signal.h>
#include <sys/signal.h>

#include "pposix/platform.hpp"
#include "pposix/util.hpp"

namespace pposix {

class sigset {
 public:
  sigset() noexcept = default;

  [[nodiscard]] inline const ::sigset_t* sigset_ptr() const noexcept { return &signals_; };

 private:
  ::sigset_t signals_{};
};

#if !PPOSIX_PLATFORM_OPENBSD
enum class sig_notify : int { none = SIGEV_NONE, signal = SIGEV_SIGNAL, thread = SIGEV_THREAD };
#endif

enum class sig_number : int {
  abort = SIGABRT,
  alarm = SIGALRM,
  undefined_memory_access = SIGBUS,
  child_terminated = SIGCHLD,
  continue_executing = SIGCONT,
  arithmetic_error = SIGFPE,
  hangup = SIGHUP,
  illegal_instruction = SIGILL,
  terminal_interrupt = SIGINT,
  kill = SIGKILL,
  pipe = SIGPIPE,
  quit = SIGQUIT,
  invalid_memory_reference = SIGSEGV,
  stop_executing = SIGSTOP,
  terminate = SIGTERM,
  terminal_stop = SIGTSTP,
  process_attempting_read = SIGTTIN,
  process_attempting_write = SIGTTOU,
  user_defined_1 = SIGUSR1,
  user_defined_2 = SIGUSR2,
  trap = SIGTRAP,
  high_bandwidth_data_available = SIGURG,
  cpu_time_limit_exceeded = SIGXCPU,
  file_size_limit_exceeded = SIGXFSZ
};

using sig_event_notify_handler = void (*)(union ::sigval);

#if !PPOSIX_PLATFORM_OPENBSD
struct sigevent : public ::sigevent {
  inline sigevent() noexcept : ::sigevent{} {}

  inline sigevent(sig_notify notify, sig_number number, sig_event_notify_handler callback,
                  int value) noexcept
      : ::sigevent{} {
    this->sigev_notify = underlying_value(notify);
    this->sigev_signo = underlying_value(number);
    this->sigev_value.sival_int = value;

    this->sigev_notify_function = callback;
    this->sigev_notify_attributes = nullptr;
  }

  inline sigevent(sig_notify notify, sig_number number, sig_event_notify_handler callback,
                  void* value) noexcept
      : ::sigevent{} {
    this->sigev_notify = underlying_value(notify);
    this->sigev_signo = underlying_value(number);
    this->sigev_value.sival_ptr = value;

    this->sigev_notify_function = callback;
    this->sigev_notify_attributes = nullptr;
  }
};
#endif

}  // namespace pposix
