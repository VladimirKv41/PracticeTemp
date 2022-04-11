cmake_minimum_required (VERSION 3.1.0)

set(CMAKE_C_COMPILER_WORKS 1)

set(CMAKE_CXX_COMPILER_WORKS 1)

set(CMAKE_CXX_STANDARD 11)

if(WIN32)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

project ("CMakeCubeTest")

if(NOT EXISTS ${GTESTDIR})
  message(SEND_ERROR "Specify GoogleTest directory")
endif()
add_subdirectory(${GTESTDIR} "gtest")


add_executable(CMakeCubeTest "test/main.cpp" "test/test.cpp" "src/cube.cpp" "src/cube.h"  
  "src/selection.cpp" "src/selection.h" "src/fact.cpp" "src/fact.h"
  "src/factclassifier.cpp" "src/factclassifier.h" "src/dimension.cpp" "src/dimension.h"
  "src/dimensionmark.cpp" "src/dimensionmark.h" "src/metric.cpp" "src/metric.h"
  "src/date.cpp" "src/date.h" "src/cubetime.cpp" "src/cubetime.h"
  "src/variant.cpp" "src/variant.h" "src/datetime.h")

target_include_directories(CMakeCubeTest PRIVATE "src")

target_link_libraries(CMakeCubeTest gtest gtest_main)

add_test(NAME CMakeCubeTest COMMAND CMakeCubeTest)
