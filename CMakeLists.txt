cmake_minimum_required(VERSION 3.10)
project(GTransforms)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

option(BUILD_LIBRARY_ONLY "Build only the transform.cpp library without the example" OFF)

add_subdirectory(external/glm)

if (BUILD_LIBRARY_ONLY)
    message(STATUS "Building only the static library")
    add_library(${PROJECT_NAME} src/lib/transform.cpp)

else()
    message(STATUS "Building the examples and the static library")
    add_library(${PROJECT_NAME} src/lib/transform.cpp)
    add_executable(TransformsDemo src/demo/main.cpp)

    target_include_directories(TransformsDemo PUBLIC external/glm include/)
    target_link_libraries(TransformsDemo glm GTransforms)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC external/glm include/)
target_link_libraries(${PROJECT_NAME} glm)