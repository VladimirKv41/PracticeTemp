
cmake_minimum_required (VERSION 2.8)

project ("CMakeCube")

set(CMAKE_CXX_STANDARD 11)

#add_subdirectory(Tests)

#add_subdirectory(gtest)

add_executable (CMakeCube "Source.cpp" "Source" "Cube.cpp" "Cube.h" "Selection.cpp" "Selection.h" "Fact.cpp" "Fact.h" "FactClassifier.cpp" "FactClassifier.h" "Dimension.cpp" "Dimension.h" "DimensionMark.cpp" "DimensionMark.h" "Metric.cpp" "Metric.h")

add_library(Cube_libs STATIC "Cube.cpp" "Selection.cpp" "Fact.cpp" "FactClassifier.cpp" "Dimension.cpp" "DimensionMark.cpp" "Metric.cpp")