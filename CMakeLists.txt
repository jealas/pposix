cmake_minimum_required(VERSION 3.14)
project(pposix)

set(CMAKE_CXX_STANDARD 17)

add_library(pposix SHARED library.cpp library.h)