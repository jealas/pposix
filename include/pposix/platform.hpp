#pragma once

namespace pposix {

// Base defines for all platforms (should be 0)
#define PPOSIX_PLATFORM_MAC_OS 0
#define PPOSIX_PLATFORM_FREE_BSD 0
#define PPOSIX_PLATFORM_SOLARIS 0
#define PPOSIX_PLATFORM_LINUX 0

enum class platform { mac_os, free_bsd, solaris, linux_ };

#if defined(__APPLE__) && __APPLE__ && defined(__MACH__) && __MACH__
inline constexpr platform compilation_platform{platform::mac_os};

#undef PPOSIX_PLATFORM_MAC_OS
#define PPOSIX_PLATFORM_MAC_OS 1

#elif defined(__FreeBSD__) && __FreeBSD__
inline constexpr platform compilation_platform{platform::free_bsd};
#undef PPOSIX_PLATFORM_FREE_BSD
#define PPOSIX_PLATFORM_FREE_BSD 1

#elif defined(__sun) && __sun
inline constexpr platform compilation_platform{platform::solaris};
#undef PPOSIX_PLATFORM_SOLARIS
#define PPOSIX_PLATFORM_SOLARIS 1

#elif defined(__linux__) && __linux__
inline constexpr platform compilation_platform{platform::linux_};
#undef PPOSIX_PLATFORM_LINUX
#define PPOSIX_PLATFORM_LINUX 1

#else
#error "Unable to detect POSIX platform."
#endif

}  // namespace pposix
