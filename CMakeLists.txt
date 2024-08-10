cmake_minimum_required(VERSION 3.10)
project(GTransforms)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
set(BUILD_SHARED_LIBS OFF)

add_subdirectory(external/glm)
add_library(${PROJECT_NAME} src/transform.cpp)

enable_testing()

add_subdirectory(tests)

target_include_directories(${PROJECT_NAME} PUBLIC external/glm include/)
target_link_libraries(${PROJECT_NAME} glm)