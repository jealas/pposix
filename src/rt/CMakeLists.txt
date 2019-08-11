add_library(
        pposix_rt

        SHARED
        mqueue.cpp
)

target_link_libraries(
        pposix_rt

        PUBLIC
        $<$<PLATFORM_ID:Linux>:rt>
        pposix
)