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

class sigevent {
 public:
 private:
};

}  // namespace pposix
