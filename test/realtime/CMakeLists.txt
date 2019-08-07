add_executable(
        test_pposix_realtime

        test_message_queue.cpp
        test_realtime_main.cpp
)

target_link_libraries(
        test_pposix_realtime

        PRIVATE
        pposix
        pposix_realtime
)

target_include_directories(
        test_pposix_realtime

        PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/catch2/single_include
)
