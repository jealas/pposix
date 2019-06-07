add_executable(
        test_pposix_capi

        test_main.cpp
        test_sockopt.cpp
)

target_link_libraries(test_pposix_capi pposix)

target_include_directories(
        test_pposix_capi

        PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/catch2/single_include
)
