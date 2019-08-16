#pragma once

#include <chrono>
#include <ratio>

namespace pposix {

using seconds = std::chrono::duration<int, std::chrono::seconds::period>;
using milliseconds = std::chrono::duration<int, std::chrono::milliseconds::period>;
using microseconds = std::chrono::duration<int, std::chrono::microseconds::period>;
using nanoseconds = std::chrono::duration<long, std::chrono::nanoseconds::period>;

}  // namespace pposix