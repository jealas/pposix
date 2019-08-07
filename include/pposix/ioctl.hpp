#pragma once

#include <memory>
#include <string_view>

#include "pposix/any_view.hpp"
#include "pposix/duration.hpp"
#include "pposix/fd.hpp"
#include "pposix/result.hpp"
#include "pposix/types.hpp"
#include "pposix/util.hpp"

namespace pposix {

// File ioctl
using ioctl_int = int;
enum class ioctl_void : int {};

enum class ioctl_request : ioctl_int {};

result<ioctl_int> ioctl(raw_fd, ioctl_request, int) noexcept;
result<ioctl_int> ioctl(raw_fd, ioctl_request, void *) noexcept;
result<ioctl_int> ioctl(raw_fd, ioctl_request, const void *) noexcept;
result<ioctl_int> ioctl(raw_fd, ioctl_request, std::nullptr_t) noexcept;
result<ioctl_int> ioctl(raw_fd, ioctl_request, any_view) noexcept;
result<ioctl_int> ioctl(raw_fd, ioctl_request, any_cview) noexcept;

}