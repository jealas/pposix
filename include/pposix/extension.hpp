#pragma once

#define _PPOSIX_MAC_OS_EXTENSION_ENABLED 0
#define _PPOSIX_FREE_BSD_EXTENSION_ENABLED 0
#define _PPOSIX_LINUX_EXTENSION_ENABLED 0
#define _PPOSIX_ANDROID_EXTENSION_ENABLED 0

#if PPOSIX_ENABLE_MAC_OS_EXTENSION
#undef _PPOSIX_MAC_OS_EXTENSION_ENABLED
#define _PPOSIX_MAC_OS_EXTENSION_ENABLED 1
#endif

#if PPOSIX_ENABLE_FREE_BSD_EXTENSION
#undef _PPOSIX_FREE_BSD_EXTENSION_ENABLED
#define _PPOSIX_FREE_BSD_EXTENSION_ENABLED 1
#endif

#ifdef PPOSIX_ENABLE_LINUX_EXTENSION
#undef _PPOSIX_LINUX_EXTENSION_ENABLED
#define _PPOSIX_LINUX_EXTENSION_ENABLED 1
#endif

#ifdef PPOSIX_ENABLE_ANDROID_EXTENSION
#undef _PPOSIX_ANDROID_EXTENSION_ENABLED
#define _PPOSIX_ANDROID_EXTENSION_ENABLED 1
#endif