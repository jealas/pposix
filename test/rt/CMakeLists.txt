add_executable(
        test_pposix_rt

        test_pposix_rt_main.cpp
        test_mqueue.cpp
)

target_link_libraries(
        test_pposix_rt

        PRIVATE
        pposix
        pposix_rt
)

target_include_directories(
        test_pposix_rt

        PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/doctest/doctest
        ${CMAKE_SOURCE_DIR}/test

)
