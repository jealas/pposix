add_executable(
        test_pposix

        test_main.cpp
        test_fd.cpp
        test_permission.cpp
)

target_link_libraries(
        test_pposix

        pposix
)

target_include_directories(
        test_pposix

        PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/catch2/single_include
)