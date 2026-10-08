cmake_minimum_required(VERSION 3.16)
project(lab2_pharma CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_COVERAGE "Enable coverage flags" OFF)

add_library(pharma_domain
    src/core.cpp
    src/orders.cpp
    src/warehouse.cpp
    src/employees.cpp
    src/transport.cpp
    src/documents.cpp
    src/reports.cpp
    src/security.cpp
    src/quality.cpp
)
target_include_directories(pharma_domain PUBLIC include)

add_executable(pharma_cli cli/main.cpp)
target_link_libraries(pharma_cli PRIVATE pharma_domain)

if(ENABLE_COVERAGE)
    target_compile_options(pharma_domain PRIVATE --coverage -O0 -g)
    target_link_options(pharma_domain PRIVATE --coverage)
endif()

enable_testing()
add_executable(test_domain tests/test_domain.cpp)
target_link_libraries(test_domain PRIVATE pharma_domain)
add_test(NAME domain_tests COMMAND test_domain)
