cmake_minimum_required(VERSION 3.9)
project(pposix)

set(CMAKE_CXX_STANDARD 17)

option(PPOSIX_LINUX "Enable Linux extensions" OFF)
option(PPOSIX_RT "Enable Real-Time extensions" OFF)

# Global library options
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(PPOSIX_EXTENSION_SRC)

# POSIX Realtime Extension Testing
if (${PPOSIX_RT})
    set(
            PPOSIX_EXTENSION_SRC

            ${PPOSIX_EXTENSION_SRC}

            src/rt/mqueue.cpp
    )
endif ()

# POSIX Linux Extension Testing
if (${PPOSIX_LINUX})
    set(
            PPOSIX_EXTENSION_SRC

            ${PPOSIX_EXTENSION_SRC}

            src/lnx/epoll.cpp
    )
endif ()

add_library(
        pposix

        src/errno.cpp
        src/fd.cpp
        src/sysconf.cpp
        src/socket.cpp
        src/file.cpp
        src/directory.cpp
        src/ioctl.cpp
        src/dirent.cpp
        src/syslog.cpp
        src/confstr.cpp
        src/signal.cpp
        src/mman.cpp
        src/fcntl.cpp
        src/dup.cpp
        src/dlfcn.cpp

        ${PPOSIX_EXTENSION_SRC}
)

target_link_libraries(
        pposix

        pthread
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_link_libraries(
            pposix

            dl
    )
endif ()

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

set(PPOSIX_COMPILE_OPTIONS -Wall -Wextra -Wpedantic -Werror -fno-rtti -Wno-unused-function)

target_compile_options(
        pposix

        PRIVATE
        ${PPOSIX_COMPILE_OPTIONS}
)

add_subdirectory(tests)