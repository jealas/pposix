cmake_minimum_required(VERSION 3.14)
project(pposix)

set(CMAKE_CXX_STANDARD 17)

add_library(
        ${CMAKE_PROJECT_NAME}

        INTERFACE
)

target_include_directories(
        ${CMAKE_PROJECT_NAME}

        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
