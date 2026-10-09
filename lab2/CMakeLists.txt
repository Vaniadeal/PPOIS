cmake_minimum_required(VERSION 3.16)
project(lab2_pharma CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_COVERAGE "Enable coverage flags" OFF)


include(FetchContent)
FetchContent_Declare(
    UnitTestPlusPlus
    GIT_REPOSITORY https://github.com/unittest-cpp/unittest-cpp.git
    GIT_TAG        v2.0.0
)
set(UTPP_USE_PLUS_PLUS_17 ON CACHE BOOL "" FORCE)
set(UTPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(UTPP_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(UnitTestPlusPlus)


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


add_executable(pharma_app cli/main.cpp)
target_link_libraries(pharma_app PRIVATE pharma_domain)


if(ENABLE_COVERAGE)
    target_compile_options(pharma_domain PUBLIC --coverage -O0 -g)
    target_link_options(pharma_domain PUBLIC --coverage)
endif()


enable_testing()
add_executable(pharma_tests tests/test_domain.cpp)
target_link_libraries(pharma_tests PRIVATE pharma_domain UnitTest++)
target_include_directories(pharma_tests PRIVATE
    ${unittestplusplus_SOURCE_DIR}
    ${unittestplusplus_SOURCE_DIR}/UnitTest++
)
add_test(NAME domain_tests COMMAND pharma_tests)