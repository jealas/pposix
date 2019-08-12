add_executable(
        test_pposix_lnx

        test_pposix_lnx_main.cpp
        test_epoll.cpp
)

target_link_libraries(
        test_pposix_lnx

        pposix
        pposix_lnx
)

target_include_directories(
        test_pposix_lnx

        PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/doctest/doctest
        ${CMAKE_SOURCE_DIR}/test
)
