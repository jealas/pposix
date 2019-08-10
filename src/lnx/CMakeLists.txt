add_library(
        pposix_lnx

        epoll.cpp
)

target_link_libraries(
        pposix_lnx

        PUBLIC
        pposix
)

set(GCC_OR_CLANG_COMPILE_OPTIONS -Wall -Wextra -Wpedantic -Werror)

target_compile_options(
        pposix_lnx

        PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:${GCC_OR_CLANG_COMPILE_OPTIONS}>
        $<$<CXX_COMPILER_ID:Clang>:${GCC_OR_CLANG_COMPILE_OPTIONS}>
)
