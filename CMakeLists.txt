cmake_minimum_required(VERSION 3.20)
project(snake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED)

add_executable(snake WIN32 main.c snake.c)
target_link_libraries(snake PRIVATE SDL3::SDL3)
