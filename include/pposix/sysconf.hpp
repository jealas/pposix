#pragma once

#include <unistd.h>

#include "pposix/platform.hpp"
#include "pposix/result.hpp"

namespace pposix {

enum class system_config_name : int {
  aio_listio_max = _SC_AIO_LISTIO_MAX,
  aio_max = _SC_AIO_MAX,
  aio_prio_delta_max = _SC_AIO_PRIO_DELTA_MAX,
  arg_max = _SC_ARG_MAX,
  atexit_max = _SC_ATEXIT_MAX,
  bc_base_max = _SC_BC_BASE_MAX,
  bc_dim_max = _SC_BC_DIM_MAX,
  bc_scale_max = _SC_BC_SCALE_MAX,
  bc_string_max = _SC_BC_STRING_MAX,
  hild_max = _SC_CHILD_MAX,
  lk_tck = _SC_CLK_TCK,
  oll_weights_max = _SC_COLL_WEIGHTS_MAX,
  delaytimer_max = _SC_DELAYTIMER_MAX,
  expr_nest_max = _SC_EXPR_NEST_MAX,
  host_name_max = _SC_HOST_NAME_MAX,
  iov_max = _SC_IOV_MAX,
  line_max = _SC_LINE_MAX,
  login_name_max = _SC_LOGIN_NAME_MAX,
  ngroups_max = _SC_NGROUPS_MAX,
  getgr_r_size_max = _SC_GETGR_R_SIZE_MAX,
  getpw_r_size_max = _SC_GETPW_R_SIZE_MAX,
  mq_open_max = _SC_MQ_OPEN_MAX,
  mq_prio_max = _SC_MQ_PRIO_MAX,
  open_max = _SC_OPEN_MAX,
  advisory_info = _SC_ADVISORY_INFO,
  barriers = _SC_BARRIERS,
  asynchronous_io = _SC_ASYNCHRONOUS_IO,
  lock_selection = _SC_CLOCK_SELECTION,
  putime = _SC_CPUTIME,
  fsync = _SC_FSYNC,
  ipv6 = _SC_IPV6,
  job_control = _SC_JOB_CONTROL,
  mapped_files = _SC_MAPPED_FILES,
  memlock = _SC_MEMLOCK,
  memlock_range = _SC_MEMLOCK_RANGE,
  memory_protection = _SC_MEMORY_PROTECTION,
  message_passing = _SC_MESSAGE_PASSING,
  monotonic_clock = _SC_MONOTONIC_CLOCK,
  prioritized_io = _SC_PRIORITIZED_IO,
  priority_scheduling = _SC_PRIORITY_SCHEDULING,
  raw_sockets = _SC_RAW_SOCKETS,
  reader_writer_locks = _SC_READER_WRITER_LOCKS,
  realtime_signals = _SC_REALTIME_SIGNALS,
  regexp = _SC_REGEXP,
  saved_ids = _SC_SAVED_IDS,
  semaphores = _SC_SEMAPHORES,
  shared_memory_objects = _SC_SHARED_MEMORY_OBJECTS,
  shell = _SC_SHELL,
  spawn = _SC_SPAWN,
  spin_locks = _SC_SPIN_LOCKS,
  sporadic_server = _SC_SPORADIC_SERVER,

#if !PPOSIX_PLATFORM_FREEBSD
  repl_max = _SC_SS_REPL_MAX,
#endif

  ynchronized_io = _SC_SYNCHRONIZED_IO,
  thread_attr_stackaddr = _SC_THREAD_ATTR_STACKADDR,
  thread_attr_stacksize = _SC_THREAD_ATTR_STACKSIZE,
  thread_cputime = _SC_THREAD_CPUTIME,
  thread_prio_inherit = _SC_THREAD_PRIO_INHERIT,
  thread_prio_protect = _SC_THREAD_PRIO_PROTECT,
  thread_priority_scheduling = _SC_THREAD_PRIORITY_SCHEDULING,
  thread_process_shared = _SC_THREAD_PROCESS_SHARED,

#if !PPOSIX_PLATFORM_MACOS && !PPOSIX_PLATFORM_FREEBSD
  thread_robust_prio_inherit = _SC_THREAD_ROBUST_PRIO_INHERIT,
  thread_robust_prio_protect = _SC_THREAD_ROBUST_PRIO_PROTECT,
#endif

  thread_safe_functions = _SC_THREAD_SAFE_FUNCTIONS,
  thread_sporadic_server = _SC_THREAD_SPORADIC_SERVER,
  threads = _SC_THREADS,
  timeouts = _SC_TIMEOUTS,
  timers = _SC_TIMERS,
  trace = _SC_TRACE,
  trace_event_filter = _SC_TRACE_EVENT_FILTER,

#if !PPOSIX_PLATFORM_FREEBSD
  trace_event_name_max = _SC_TRACE_EVENT_NAME_MAX,
#endif

  trace_inherit = _SC_TRACE_INHERIT,
  trace_log = _SC_TRACE_LOG,

#if !PPOSIX_PLATFORM_FREEBSD
  trace_name_max = _SC_TRACE_NAME_MAX,
  trace_sys_max = _SC_TRACE_SYS_MAX,
  trace_user_event_max = _SC_TRACE_USER_EVENT_MAX,
#endif

  typed_memory_objects = _SC_TYPED_MEMORY_OBJECTS,
  version = _SC_VERSION,

#if !PPOSIX_PLATFORM_MACOS && !PPOSIX_PLATFORM_FREEBSD
  v7_ilp32_off32 = _SC_V7_ILP32_OFF32,
#endif

#if !PPOSIX_PLATFORM_MACOS && !PPOSIX_PLATFORM_FREEBSD
  v7_ilp32_offbig = _SC_V7_ILP32_OFFBIG,
  v7_lp64_off64 = _SC_V7_LP64_OFF64,
  v7_lpbig_offbig = _SC_V7_LPBIG_OFFBIG,
#endif

  v6_ilp32_off32 = _SC_V6_ILP32_OFF32,
  v6_ilp32_offbig = _SC_V6_ILP32_OFFBIG,
  v6_lp64_off64 = _SC_V6_LP64_OFF64,
  v6_lpbig_offbig = _SC_V6_LPBIG_OFFBIG,
  c_bind_2 = _SC_2_C_BIND,
  c_dev_2 = _SC_2_C_DEV,
  char_term_2 = _SC_2_CHAR_TERM,
  fort_dev_2 = _SC_2_FORT_DEV,
  fort_run_2 = _SC_2_FORT_RUN,
  localedef_2 = _SC_2_LOCALEDEF,
  pbs_2 = _SC_2_PBS,
  pbs_accounting_2 = _SC_2_PBS_ACCOUNTING,
  pbs_checkpoint_2 = _SC_2_PBS_CHECKPOINT,
  pbs_locate_2 = _SC_2_PBS_LOCATE,
  pbs_message_2 = _SC_2_PBS_MESSAGE,
  pbs_track_2 = _SC_2_PBS_TRACK,
  sw_dev_2 = _SC_2_SW_DEV,
  upe_2 = _SC_2_UPE,
  version_2 = _SC_2_VERSION,
  page_size = _SC_PAGE_SIZE,
  pagesize = _SC_PAGESIZE,
  thread_destructor_iterations = _SC_THREAD_DESTRUCTOR_ITERATIONS,
  thread_keys_max = _SC_THREAD_KEYS_MAX,
  thread_stack_min = _SC_THREAD_STACK_MIN,
  thread_threads_max = _SC_THREAD_THREADS_MAX,
  re_dup_max = _SC_RE_DUP_MAX,
  rtsig_max = _SC_RTSIG_MAX,
  em_nsems_max = _SC_SEM_NSEMS_MAX,
  em_value_max = _SC_SEM_VALUE_MAX,
  igqueue_max = _SC_SIGQUEUE_MAX,
  tream_max = _SC_STREAM_MAX,
  ymloop_max = _SC_SYMLOOP_MAX,
  timer_max = _SC_TIMER_MAX,
  tty_name_max = _SC_TTY_NAME_MAX,
  tzname_max = _SC_TZNAME_MAX,
  xopen_crypt = _SC_XOPEN_CRYPT,
  xopen_enh_i18n = _SC_XOPEN_ENH_I18N,
  xopen_realtime = _SC_XOPEN_REALTIME,
  xopen_realtime_threads = _SC_XOPEN_REALTIME_THREADS,
  xopen_shm = _SC_XOPEN_SHM,
  xopen_streams = _SC_XOPEN_STREAMS,
  xopen_unix = _SC_XOPEN_UNIX,
  xopen_version = _SC_XOPEN_VERSION,
};

using system_config_value = long;

result<system_config_value> sysconf(system_config_name config_name) noexcept;

}  // namespace pposix
