#pragma once

#include <cstddef>

#include <unistd.h>

#include "pposix/char_span.hpp"
#include "pposix/result.hpp"

namespace pposix {

enum class confstr_name : int {
  path = _CS_PATH,
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

// TODO: Remove temporary solution for missing flags on Linux.
#ifdef _CS_POSIX_V7_THREADS_CFLAGS
  v7_threads_cflags = _CS_POSIX_V7_THREADS_CFLAGS,
#endif

#ifdef _CS_POSIX_V7_THREADS_LDFLAGS
  v7_threads_ldflags = _CS_POSIX_V7_THREADS_LDFLAGS,
#endif

  v7_width_restricted_envs = _CS_POSIX_V7_WIDTH_RESTRICTED_ENVS,
  v7_env = _CS_V7_ENV,
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
  v6_env = _CS_V6_ENV
};

result<size_t> confstr_length(confstr_name name) noexcept;

result<char_span> confstr(confstr_name name, char_span name_buffer) noexcept;

}  // namespace pposix
