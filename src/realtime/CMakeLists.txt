add_library(
        pposix_realtime

        SHARED
        message_queue.cpp
)

target_link_libraries(
        pposix_realtime

        PUBLIC
        $<$<PLATFORM_ID:Linux>:rt>
        pposix
)