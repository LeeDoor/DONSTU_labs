cmake_minimum_required(VERSION 3.14)
project(DArray VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_BUILD_TYPE Debug)

option(BUILD_TESTS "Собрать тесты" ON)
option(DOWNLOAD_GTEST "Скачать Google Test, если не найден в системе" ON)

set(SOURCES 
    src/d_array.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES} src/main.cpp)
target_compile_options(${PROJECT_NAME} PUBLIC "-g")
find_package(GTest)
include(FetchContent)
if(NOT ${GTest_FOUND})
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.17.0
        FIND_PACKAGE_ARGS NAMES GTest
    )
    FetchContent_MakeAvailable(googletest)
endif()

enable_testing()
add_executable(testing
    src/DArrayTest.cpp
    src/DArrayIteratorTest.cpp
    ${SOURCES}
)
target_link_libraries(testing PUBLIC
GTest::gtest_main GTest::gmock
)
