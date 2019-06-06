add_executable(
        test_pposix_posix

        test_main.cpp
        test_socket.cpp
)

target_link_libraries(test_pposix_posix pposix)

target_include_directories(
        test_pposix_posix

        PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/catch2/single_include
)
