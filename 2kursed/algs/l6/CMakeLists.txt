cmake_minimum_required(VERSION 3.16)

project(lab6_variant19 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(lab6_variant19 main.cpp)

target_compile_options(lab6_variant19 PRIVATE
    -Wall
    -Wextra
    -pedantic
)

target_sources(lab6_variant19 PRIVATE
    array_list.hpp
    array_stack.hpp
    lab_runner.hpp
    linked_list.hpp
    linked_stack.hpp
    stack_concepts.hpp
)
