add_executable(
        pposix_tests

        pt.cpp
        main.cpp
        test_pposix.cpp
)


set(PPOSIX_TEST_COMPILE_OPTIONS -Wall -Wextra -Wpedantic -Werror -fno-rtti)

target_compile_options(
        pposix_tests

        PRIVATE
        ${PPOSIX_TEST_COMPILE_OPTIONS}
)

target_link_libraries(
        pposix_tests

        pposix
)
