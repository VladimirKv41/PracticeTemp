cmake_minimum_required (VERSION 2.8)

project ("CMakeCubeTest")

add_executable(CMakeCubeTest "main.cpp" "test.cpp")

add_test(NAME CMakeCubeTest COMMAND CMakeCubeTest)

target_link_libraries(CMakeCubeTest gtest gtest_main Cube_libs)