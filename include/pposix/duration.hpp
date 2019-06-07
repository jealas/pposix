#pragma once

#include <chrono>
#include <ratio>

namespace pposix {

using seconds = std::chrono::duration<int, std::chrono::seconds::period>;
using microseconds = std::chrono::duration<int, std::chrono::microseconds::period>;

}  // namespace pposix