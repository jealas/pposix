cmake_minimum_required(VERSION 3.14)
project(pposix)

set(CMAKE_CXX_STANDARD 17)

add_library(
        ${CMAKE_PROJECT_NAME}

        src/errno.cpp
        src/default_close_policy.cpp
        src/sysconf.cpp
)

target_include_directories(
        ${CMAKE_PROJECT_NAME}

        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Testing
add_subdirectory(test)
