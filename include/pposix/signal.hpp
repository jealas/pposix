#pragma once

#include <signal.h>

namespace pposix {

class sigset {
 public:
  sigset() noexcept = default;

  [[nodiscard]] inline const ::sigset_t *sigset_ptr() const noexcept { return &signals_; };

 private:
  ::sigset_t signals_{};
};

enum class sig_notify : int { none = SIGEV_NONE, signal = SIGEV_SIGNAL, thread = SIGEV_THREAD };

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

class sigevent : public ::sigevent {
 public:
 private:
};

}  // namespace pposix
