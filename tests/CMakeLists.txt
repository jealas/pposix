set(PPOSIX_TEST_COMPILE_OPTIONS -Wall -Wextra -Wpedantic -Werror -fno-rtti)

add_library(
        libpt

        STATIC
        pt.cpp
)


target_link_libraries(
        libpt

        pposix
)

target_compile_options(
        libpt

        PRIVATE
        ${PPOSIX_TEST_COMPILE_OPTIONS}
)

add_executable(
        pt

        main.cpp
)


target_link_libraries(
        pt

        libpt
        pposix

        pthread
)

if (APPLE OR LINUX)
    target_link_libraries(
            pt

            dl
    )
endif ()

target_compile_options(
        pt

        PRIVATE
        ${PPOSIX_TEST_COMPILE_OPTIONS}
)

add_library(
        test_pposix

        MODULE

        test_pposix.cpp
)

target_link_libraries(
        test_pposix

        libpt
)


target_compile_options(
        test_pposix

        PRIVATE
        ${PPOSIX_TEST_COMPILE_OPTIONS}
)
