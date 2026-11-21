cmake_minimum_required(VERSION 3.16)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(lab4_generic_programming CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_COVERAGE "Enable code coverage flags" OFF)

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

if(TARGET TestUnitTest++)
    target_compile_options(TestUnitTest++ PRIVATE -Wno-error)
endif()

add_library(models_lib src/models/Student.cpp)
target_include_directories(models_lib PUBLIC include)

add_executable(lab4_app cli/main.cpp)
target_include_directories(lab4_app PRIVATE include)
target_link_libraries(lab4_app PRIVATE models_lib)

enable_testing()

add_executable(sorts_tests tests/test_sorts.cpp)
target_include_directories(sorts_tests PRIVATE
    include
    ${unittestplusplus_SOURCE_DIR}
    ${unittestplusplus_SOURCE_DIR}/UnitTest++
)
target_link_libraries(sorts_tests PRIVATE models_lib UnitTest++)
add_test(NAME sorts_tests COMMAND sorts_tests)

add_executable(graph_tests tests/test_graph.cpp)
target_include_directories(graph_tests PRIVATE
    include
    ${unittestplusplus_SOURCE_DIR}
    ${unittestplusplus_SOURCE_DIR}/UnitTest++
)
target_link_libraries(graph_tests PRIVATE UnitTest++)
add_test(NAME graph_tests COMMAND graph_tests)

if(ENABLE_COVERAGE)
    message(STATUS "Coverage is enabled")
    target_compile_options(models_lib PRIVATE --coverage -O0 -g)
    target_link_options(models_lib PRIVATE --coverage)
    target_link_options(lab4_app PRIVATE --coverage)
    target_compile_options(sorts_tests PRIVATE --coverage -O0 -g)
    target_link_options(sorts_tests PRIVATE --coverage)
    target_compile_options(graph_tests PRIVATE --coverage -O0 -g)
    target_link_options(graph_tests PRIVATE --coverage)
endif()

