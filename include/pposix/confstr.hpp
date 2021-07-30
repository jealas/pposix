#pragma once

#include <unistd.h>

#include <cstddef>

#include "pposix/char_span.hpp"
#include "pposix/platform.hpp"
#include "pposix/result.hpp"

namespace pposix {

enum class confstr_name : int {
  path = _CS_PATH,

#if !PPOSIX_PLATFORM_MACOS && !PPOSIX_PLATFORM_FREEBSD
  v7_ilp32_off32_cflags = _CS_POSIX_V7_ILP32_OFF32_CFLAGS,
  v7_ilp32_off32_ldflags = _CS_POSIX_V7_ILP32_OFF32_LDFLAGS,
  v7_ilp32_off32_libs = _CS_POSIX_V7_ILP32_OFF32_LIBS,
  v7_ilp32_offbig_cflags = _CS_POSIX_V7_ILP32_OFFBIG_CFLAGS,
  v7_ilp32_offbig_ldflags = _CS_POSIX_V7_ILP32_OFFBIG_LDFLAGS,
  v7_ilp32_offbig_libs = _CS_POSIX_V7_ILP32_OFFBIG_LIBS,
  v7_lp64_off64_cflags = _CS_POSIX_V7_LP64_OFF64_CFLAGS,
  v7_lp64_off64_ldflags = _CS_POSIX_V7_LP64_OFF64_LDFLAGS,
  v7_lp64_off64_libs = _CS_POSIX_V7_LP64_OFF64_LIBS,
  v7_lpbig_offbig_cflags = _CS_POSIX_V7_LPBIG_OFFBIG_CFLAGS,
  v7_lpbig_offbig_ldflags = _CS_POSIX_V7_LPBIG_OFFBIG_LDFLAGS,
  v7_lpbig_offbig_libs = _CS_POSIX_V7_LPBIG_OFFBIG_LIBS,
#endif

#if !PPOSIX_PLATFORM_LINUX && !PPOSIX_PLATFORM_MACOS && !PPOSIX_PLATFORM_FREEBSD
  v7_threads_cflags = _CS_POSIX_V7_THREADS_CFLAGS,
  v7_threads_ldflags = _CS_POSIX_V7_THREADS_LDFLAGS,
#endif

#if !PPOSIX_PLATFORM_MACOS && !PPOSIX_PLATFORM_FREEBSD
  v7_width_restricted_envs = _CS_POSIX_V7_WIDTH_RESTRICTED_ENVS,
  v7_env = _CS_V7_ENV,
#endif

  v6_ilp32_off32_cflags = _CS_POSIX_V6_ILP32_OFF32_CFLAGS,
  v6_ilp32_off32_ldflags = _CS_POSIX_V6_ILP32_OFF32_LDFLAGS,
  v6_ilp32_off32_libs = _CS_POSIX_V6_ILP32_OFF32_LIBS,
  v6_ilp32_offbig_cflags = _CS_POSIX_V6_ILP32_OFFBIG_CFLAGS,
  v6_ilp32_offbig_ldflags = _CS_POSIX_V6_ILP32_OFFBIG_LDFLAGS,
  v6_ilp32_offbig_libs = _CS_POSIX_V6_ILP32_OFFBIG_LIBS,
  v6_lp64_off64_cflags = _CS_POSIX_V6_LP64_OFF64_CFLAGS,
  v6_lp64_off64_ldflags = _CS_POSIX_V6_LP64_OFF64_LDFLAGS,
  v6_lp64_off64_libs = _CS_POSIX_V6_LP64_OFF64_LIBS,
  v6_lpbig_offbig_cflags = _CS_POSIX_V6_LPBIG_OFFBIG_CFLAGS,
  v6_lpbig_offbig_ldflags = _CS_POSIX_V6_LPBIG_OFFBIG_LDFLAGS,
  v6_lpbig_offbig_libs = _CS_POSIX_V6_LPBIG_OFFBIG_LIBS,
  v6_width_restricted_envs = _CS_POSIX_V6_WIDTH_RESTRICTED_ENVS,

#if !PPOSIX_PLATFORM_MACOS && !PPOSIX_PLATFORM_FREEBSD
  v6_env = _CS_V6_ENV
#endif
};

result<size_t> confstr_length(confstr_name name) noexcept;

result<char_span> confstr(confstr_name name, char_span name_buffer) noexcept;

}  // namespace pposix
