#pragma once

#include <signal.h>

namespace pposix {

class sigset {
 public:
  sigset() noexcept = default;

  [[nodiscard]] const ::sigset_t *sigset_ptr() const noexcept;

 private:
  ::sigset_t signals_{};
};

}  // namespace pposix
