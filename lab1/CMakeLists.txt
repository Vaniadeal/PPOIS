cmake_minimum_required(VERSION 3.16)
project(ppois_lab1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Библиотека с классами (Вектор и Многочлен)
add_library(lab1_lib
    src/Vector.cpp
    src/Polynomial.cpp
)
target_include_directories(lab1_lib PUBLIC include)

# Консольное приложение с меню
add_executable(lab1_app app/main.cpp)
target_link_libraries(lab1_app PRIVATE lab1_lib)

# Тесты (позже подключим UnitTest++)
# add_executable(lab1_tests tests/test_main.cpp)
# target_link_libraries(lab1_tests PRIVATE lab1_lib)
