cmake_minimum_required(VERSION 3.16)

project(lab_memory_manager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(lab_memory_manager
    main.cpp
    memory_manager.cpp
)
