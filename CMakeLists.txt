cmake_minimum_required (VERSION 3.1.0)

set(CMAKE_C_COMPILER_WORKS 1)

set(CMAKE_CXX_COMPILER_WORKS 1)

set(CMAKE_CXX_STANDARD 11)

if(WIN32)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	message("Specify GoogleTest directory")
endif()

project ("CMakeCubeTest")

if(NOT EXISTS ${GTESTDIR})
  message(SEND_ERROR "Specify GoogleTest directory")
endif()
add_subdirectory(${GTESTDIR} "gtest")

add_executable(CMakeCubeTest "main.cpp" "test.cpp" "cube.cpp" "cube.h"  
  "selection.cpp" "selection.h" "fact.cpp" "fact.h"
  "factclassifier.cpp" "factclassifier.h" "dimension.cpp" "dimension.h"
  "dimensionmark.cpp" "dimensionmark.h" "metric.cpp" "metric.h"
  "date.cpp" "date.h" "time.cpp" "time.h"
  "variant.cpp" "variant.h" "datetime.h")

target_link_libraries(CMakeCubeTest gtest gtest_main)

add_test(NAME CMakeCubeTest COMMAND CMakeCubeTest)
