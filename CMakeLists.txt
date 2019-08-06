cmake_minimum_required(VERSION 3.14)
project(pposix)

set(CMAKE_CXX_STANDARD 17)

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
)

target_include_directories(
        ${CMAKE_PROJECT_NAME}

        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Testing
add_subdirectory(test)
