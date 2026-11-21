cmake_minimum_required(VERSION 3.31)
project(mini-lua-cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(warning-options INTERFACE)
target_compile_options(warning-options INTERFACE -Wall -Wextra -Wpedantic -Wconversion -Wfloat-equal -Wshadow)

set(SOURCES
    src/errors.cpp
    src/location.cpp
    src/main.cpp
    src/scanner.cpp
    src/token.cpp
)

add_executable(mini-lua-cpp ${SOURCES})
target_link_libraries(mini-lua-cpp PRIVATE warning-options)
target_compile_options(mini-lua-cpp PRIVATE -O2)

add_executable(mini-lua-cpp-debug ${SOURCES})
target_link_libraries(mini-lua-cpp-debug PRIVATE warning-options)
target_compile_options(mini-lua-cpp-debug PRIVATE -O0 -g -fsanitize=address,undefined,bounds)
target_link_options(mini-lua-cpp-debug PRIVATE -fsanitize=address,undefined,bounds)
