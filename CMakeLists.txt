cmake_minimum_required(VERSION 3.16)

project(camera_preview LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

add_library(${PROJECT_NAME}
    src/application/application.cpp
    src/core/latest_frame_buffer.cpp
    src/camera/camera_source_opencv.cpp
    src/camera/device_validator_linux.cpp
    src/renderer/renderer_opencv.cpp
    src/cli/commandline_arguments_parser.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include/camera_preview
        ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        ${OpenCV_LIBS}
        Threads::Threads
)

add_executable(${PROJECT_NAME}_app
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}_app
    PRIVATE
        ${PROJECT_NAME}
)

include(CTest)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()