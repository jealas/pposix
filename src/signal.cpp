#pragma once

namespace pposix {

const ::sigset_t *sigset::sigset_ptr() const noexcept { return &signals_; }

}  // namespace pposix
