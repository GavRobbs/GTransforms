cmake_minimum_required(VERSION 3.16)
project(GTransforms)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

option(GTRANSFORMS_BUILD_TESTS "Run the unit tests" OFF)

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:DebugDLL>")
endif()

set(BUILD_SHARED_LIBS OFF)

add_subdirectory(external/glm)
add_library(${PROJECT_NAME} src/transform.cpp)

if(GTRANSFORMS_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC external/glm include/)
target_link_libraries(${PROJECT_NAME} glm)