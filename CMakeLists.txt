cmake_minimum_required(VERSION 3.16)

project(camera_preview LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cli/commandline_arguments_parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/camera/device_validator_linux.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/application/application.cpp
)

find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME}
    ${SOURCES}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${OpenCV_LIBS}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${OpenCV_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include/camera_preview
)