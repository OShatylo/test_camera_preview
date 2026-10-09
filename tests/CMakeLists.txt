find_package(GTest REQUIRED)

add_executable(camera_preview_tests
    application_test.cpp
)

target_include_directories(camera_preview_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(camera_preview_tests
    PRIVATE
        ${PROJECT_NAME}
        GTest::gtest
        GTest::gmock
        GTest::gtest_main
)

add_test(
    NAME camera_preview_tests
    COMMAND camera_preview_tests
)