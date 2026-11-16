cmake_minimum_required(VERSION 3.16)

project(chip8 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL3 REQUIRED CONFIG)

add_executable(chip8 
    src/main.c 
    src/chip8.c
    src/platform.c
    src/utils.c
)

target_link_libraries(chip8 PRIVATE SDL3::SDL3)
target_include_directories(chip8 PRIVATE include)