# CMakeList.txt: проект CMake для CMakeCube;

cmake_minimum_required (VERSION 2.8)

project (CMakeCube)

include_directories(${PROJECT_SOURCE_DIR}/include/)

add_library(Cube STATIC "Cube.cpp")
add_library(Selection STATIC "Selection.cpp")
add_library(Fact STATIC "Fact.cpp")
add_library(FactClassifier STATIC "FactClassifier.cpp")
add_library(Dimension STATIC "Dimension.cpp")
add_library(DimensionMark STATIC "DimensionMark.cpp")
add_library(Metric STATIC "Metric.cpp")

add_executable ( CMakeCube	"Source.cpp")

target_link_libraries(CMakeCube Cube Fact FactClassifier 
 Selection Dimension DimensionMark Metric)
