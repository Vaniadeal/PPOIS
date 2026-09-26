cmake_minimum_required(VERSION 3.16)
project(ppois_lab1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Библиотека с классами
add_library(lab1_lib
    src/Vector.cpp
    src/Polynomial.cpp
)
target_include_directories(lab1_lib PUBLIC include)

# Консольное приложение
add_executable(lab1_app app/main.cpp)
target_link_libraries(lab1_app PRIVATE lab1_lib)

# UnitTest++ через FetchContent
include(FetchContent)

FetchContent_Declare(
    unittestpp
    GIT_REPOSITORY https://github.com/unittest-cpp/unittest-cpp.git
    GIT_TAG        v2.0.0
)
FetchContent_GetProperties(unittestpp)
if(NOT unittestpp_POPULATED)
    FetchContent_Populate(unittestpp)
    add_subdirectory(${unittestpp_SOURCE_DIR} ${unittestpp_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# Тесты
add_executable(lab1_tests tests/test_main.cpp)
target_link_libraries(lab1_tests PRIVATE lab1_lib UnitTest++)

# Явно добавляем include-путь к заголовкам UnitTest++
target_include_directories(lab1_tests PRIVATE ${unittestpp_SOURCE_DIR})