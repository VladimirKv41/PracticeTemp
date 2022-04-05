cmake_minimum_required (VERSION 3.0.0)

set(CMAKE_C_COMPILER_WORKS 1)

set(CMAKE_CXX_COMPILER_WORKS 1)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

set(CMAKE_CXX_STANDARD 11)

project ("CMakeCubeTest")

add_subdirectory(gtest)

add_executable(CMakeCubeTest "main.cpp" "test.cpp" "cube.cpp" "cube.h"  
	"selection.cpp" "selection.h" "fact.cpp" "fact.h"
	"factclassifier.cpp" "factclassifier.h" "dimension.cpp" "dimension.h"
	"dimensionmark.cpp" "dimensionmark.h" "metric.cpp" "metric.h"
	"date.cpp" "date.h" "time.cpp" "time.h"
	"variant.cpp" "variant.h")

add_test(NAME CMakeCubeTest COMMAND CMakeCubeTest)

target_link_libraries(CMakeCubeTest gtest gtest_main)