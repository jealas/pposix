set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
#set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

set(PPOSIX_TEST_COMPILE_OPTIONS -Wall -Wextra -Wpedantic -Werror -fno-rtti)

add_library(
        libpt

        STATIC
        pt.cpp
)

target_compile_features(
        libpt

        PUBLIC
        cxx_std_14
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

        test_compiles
)

add_library(
        test_compiles

        MODULE

        test_compiles.cpp
)

target_link_libraries(
        test_compiles

        libpt
)


target_compile_options(
        test_compiles

        PRIVATE
        ${PPOSIX_TEST_COMPILE_OPTIONS}
)
