add_executable(
        pposix_tests

        main.cpp
)


set(PPOSIX_TEST_COMPILE_OPTIONS -Wall -Wextra -Wpedantic -Werror)

target_compile_options(
        pposix_tests

        PRIVATE
        ${PPOSIX_TEST_COMPILE_OPTIONS}
)

target_link_libraries(
        pposix_tests

        pposix
)
