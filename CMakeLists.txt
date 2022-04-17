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

if(NOT EXISTS ${BUILDDIR})
  message(SEND_ERROR "Specify build directory")
endif()

add_executable(CMakeCubeTest "test/main.cpp" "test/test.cpp" "src/cube.cpp" "src/cube.h"  
  "src/selection.cpp" "src/selection.h" "src/fact.cpp" "src/fact.h"
  "src/factclassifier.cpp" "src/factclassifier.h" "src/dimension.cpp" "src/dimension.h"
  "src/dimensionmark.cpp" "src/dimensionmark.h" "src/metric.cpp" "src/metric.h"
  "src/date.cpp" "src/date.h" "src/cubetime.cpp" "src/cubetime.h"
  "src/variant.cpp" "src/variant.h" "src/datetime.h")

target_include_directories(CMakeCubeTest PRIVATE "src")
target_include_directories(CMakeCubeTest PRIVATE "${GTESTDIR}/googletest/include")

if(MINGW OR UNIX)
target_link_libraries(CMakeCubeTest 
"${BUILDDIR}/lib/libgtest.a" 
"${BUILDDIR}/lib/libgtest_main.a" -pthread )
elseif(MSVC)
target_link_libraries(CMakeCubeTest 
"${BUILDDIR}/lib/gtestd.lib" 
"${BUILDDIR}/lib/gtest_maind.lib" -pthread )
endif()

add_test(NAME CMakeCubeTest COMMAND CMakeCubeTest)
