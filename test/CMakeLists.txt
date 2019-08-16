set(
        PPOSIX_REALTIME_EXTENSION_TEST_SRC

        rt/test_mqueue.cpp
)

set(
        PPOSIX_LINUX_EXTENSION_TEST_SRC

        lnx/test_epoll.cpp
)

add_executable(
        test_pposix

        test_pposix_main.cpp
        test_unique_fd.cpp
        test_permission.cpp
        test_buffer_span.cpp
        test_result.cpp
        test_any_view.cpp
        test_descriptor.cpp
        test_unique_descriptor.cpp

        test_getsockopt.cpp
        test_permission.cpp
        test_sysconf.cpp
        test_select.cpp

        # POSIX Realtime Extension
        $<$<BOOL:${PPOSIX_REALTIME_EXTENSION_ENABLED}>:${PPOSIX_REALTIME_EXTENSION_TEST_SRC}>

        # POSIX Linux Extension
        $<$<BOOL:${PPOSIX_LINUX_EXTENSION_ENABLED}>:${PPOSIX_LINUX_EXTENSION_TEST_SRC}>
)

target_link_libraries(
        test_pposix

        pposix
        $<$<BOOL:${PPOSIX_REALTIME_EXTENSION_ENABLED}>:pposix_rt>
        $<$<BOOL:${PPOSIX_LINUX_EXTENSION_ENABLED}>:pposix_lnx>
)

target_include_directories(
        test_pposix

        PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/doctest/doctest
        ${CMAKE_SOURCE_DIR}/test
)

