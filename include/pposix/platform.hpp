#pragma once

namespace pposix {

// Base defines for all platforms (should be 0)
#define PPOSIX_PLATFORM_MACOS 0
#define PPOSIX_PLATFORM_FREEBSD 0
#define PPOSIX_PLATFORM_OPENBSD 0
#define PPOSIX_PLATFORM_SOLARIS 0
#define PPOSIX_PLATFORM_LINUX 0

enum class platform { macos, freebsd, openbsd, solaris, linux_ };

#if defined(__APPLE__) && __APPLE__ && defined(__MACH__) && __MACH__
inline constexpr platform compilation_platform{platform::macos};

#undef PPOSIX_PLATFORM_MACOS
#define PPOSIX_PLATFORM_MACOS 1

#elif defined(__FreeBSD__) && __FreeBSD__
inline constexpr platform compilation_platform{platform::freebsd};
#undef PPOSIX_PLATFORM_FREEBSD
#define PPOSIX_PLATFORM_FREEBSD 1

#elif defined(__OpenBSD__) && __OpenBSD__
inline constexpr platform compilation_platform{platform::openbsd};
#undef PPOSIX_PLATFORM_OPENBSD
#define PPOSIX_PLATFORM_OPENBSD 1

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
