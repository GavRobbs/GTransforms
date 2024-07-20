cmake_minimum_required(VERSION 3.10)
project(TransformsDemo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include_directories(include)

add_executable(${PROJECT_NAME} src/main.cpp src/transform.cpp)

add_subdirectory(external/glm)
target_link_libraries(${PROJECT_NAME} glm)