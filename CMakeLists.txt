cmake_minimum_required(VERSION 3.9)
project(pposix)

set(CMAKE_CXX_STANDARD 17)

# Global library options
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(
        ${CMAKE_PROJECT_NAME}

        src/errno.cpp
        src/unique_fd.cpp
        src/sysconf.cpp
        src/socket.cpp
        src/file.cpp
        src/ioctl.cpp
        src/dirent.cpp
        src/syslog.cpp
        src/confstr.cpp
        src/signal.cpp
)

target_include_directories(
        ${CMAKE_PROJECT_NAME}

        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(GCC_OR_CLANG_COMPILE_OPTIONS -Wall -Wextra -Wpedantic -Werror)

target_compile_options(
        ${CMAKE_PROJECT_NAME}

        PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:${GCC_OR_CLANG_COMPILE_OPTIONS}>
        $<$<CXX_COMPILER_ID:Clang>:${GCC_OR_CLANG_COMPILE_OPTIONS}>
)

# POSIX Realtime Extension Testing
if (${PPOSIX_REALTIME_EXTENSION_ENABLED})
    add_subdirectory(src/rt)
endif ()

# POSIX Linux Extension Testing
if (${PPOSIX_LINUX_EXTENSION_ENABLED})
    add_subdirectory(src/lnx)
endif ()

# POSIX Testing
add_subdirectory(test)
