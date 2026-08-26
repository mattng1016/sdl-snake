cmake_minimum_required(VERSION 3.16)
project(snake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

find_package(SDL3 CONFIG REQUIRED)

# Create your game executable target as usual
add_executable(snake WIN32 main.c snake.c)

# Link to the actual SDL3 library.
target_link_libraries(snake PRIVATE SDL3::SDL3)
