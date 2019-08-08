add_executable(
        test_pposix_rt

        test_message_queue.cpp
        test_pposix_rt_main.cpp
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
        ${CMAKE_SOURCE_DIR}/lib/catch2/single_include
)
