#pragma once

namespace pposix {

// Base defines for all platforms (should be 0)
#define PPOSIX_PLATFORM_MAC_OS 0
#define PPOSIX_PLATFORM_ANDROID 0
#define PPOSIX_PLATFORM_LINUX 0
#define PPOSIX_PLATFORM_UNIX 0
#define PPOSIX_PLATFORM_POSIX 0

enum class platform { mac_os, linux_, android, unix_, posix };

#if defined(__APPLE__) && __APPLE__ && defined(__MACH__) && __MACH__
inline constexpr platform compilation_platform{platform::mac_os};

#undef PPOSIX_PLATFORM_MAC_OS
#define PPOSIX_PLATFORM_MAC_OS 1

#elif defined(__ANDROID__) && __ANDROID__
inline constexpr platform compilation_platform{platform::android};

#undef PPOSIX_PLATFORM_ANDROID
#define PPOSIX_PLATFORM_ANDROID 1

#elif defined(__linux__) && __linux__
inline constexpr platform compilation_platform{platform::linux_};
#undef PPOSIX_PLATFORM_LINUX
#define PPOSIX_PLATFORM_LINUX 1

#elif defined(__unix__) && __unix__
inline constexpr platform compilation_platform{platform::unix_};

#undef PPOSIX_PLATFORM_UNIX
#define PPOSIX_PLATFORM_UNIX 1

#elif defined(_POSIX_VERSION) && _POSIX_VERSION
inline constexpr platform compilation_platform{platform::posix};

#undef PPOSIX_PLATFORM_POSIX
#define PPOSIX_PLATFORM_POSIX 1

#else
#error "Unable to detect POSIX platform."
#endif

}  // namespace pposix
