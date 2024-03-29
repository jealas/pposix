cmake_minimum_required(VERSION 3.9)
project(pposix)

set(CMAKE_CXX_STANDARD 17)

option(PPOSIX_LINUX "Enable Linux extensions" OFF)
option(PPOSIX_RT "Enable Real-Time extensions" OFF)

# Global library options
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(
        pposix

        src/errno.cpp
        src/file_descriptor.cpp
        src/sysconf.cpp
        src/socket.cpp
        src/file.cpp
        src/ioctl.cpp
        src/dirent.cpp
        src/syslog.cpp
        src/confstr.cpp
        src/signal.cpp
        src/mman.cpp
        src/fcntl.cpp
)

target_compile_definitions(
        pposix

        PUBLIC
        PPOSIX_LINUX_EXTENSION_ENABLED=$<BOOL:${PPOSIX_LINUX}>
        PPOSIX_REALTIME_EXTENSION_ENABLED=$<BOOL:${PPOSIX_RT}>
)

target_include_directories(
        pposix

        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(GCC_OR_CLANG_COMPILE_OPTIONS -Wall -Wextra -Wpedantic -Werror)

target_compile_options(
        pposix

        PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:${GCC_OR_CLANG_COMPILE_OPTIONS}>
        $<$<CXX_COMPILER_ID:Clang>:${GCC_OR_CLANG_COMPILE_OPTIONS}>
)

# POSIX Realtime Extension Testing
if (${PPOSIX_RT})
    add_subdirectory(src/rt)
endif ()

# POSIX Linux Extension Testing
if (${PPOSIX_LINUX})
    add_subdirectory(src/lnx)
endif ()
