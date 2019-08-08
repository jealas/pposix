add_executable(
        test_pposix

        test_pposix_main.cpp
        test_unique_fd.cpp
        test_permission.cpp
        test_buffer_span.cpp
        test_result.cpp
        test_any_view.cpp

        test_getsockopt.cpp
        test_permission.cpp
        test_sysconf.cpp
)

target_link_libraries(
        test_pposix

        pposix
)

target_include_directories(
        test_pposix

        PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/catch2/single_include
)
