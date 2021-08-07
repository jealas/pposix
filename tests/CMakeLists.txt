set(PPOSIX_TEST_COMPILE_OPTIONS -Wall -Wextra -Wpedantic -Werror -fno-rtti)

add_library(
        libpt

        STATIC
        pt.cpp
)

target_compile_features(
        libpt

        PUBLIC
        cxx_std_11
)

set_target_properties(
        libpt

        PROPERTIES
        CXX_EXTENSIONS OFF
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

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
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

add_dependencies(
        pt

        test_pposix
)

add_library(
        test_pposix

        MODULE

        test_pposix.cpp
        test_compiles.cpp
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

add_executable(
        allocations_before_main

        test_allocations_before_main.cpp
)

target_link_libraries(
        allocations_before_main
)
