cmake_minimum_required(VERSION 3.16)
project(Multivariate_Polynomials)

set(CMAKE_CXX_STANDARD 17)

# 'lib' is the folder with Google Test sources
add_subdirectory(GoogleTest/lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


add_library(Multivariate_Polynomials point.cpp point.h Multiindex.cpp Multiindex.h)

# 'Google_Tests_run' is the target name
add_executable(Google_Tests_run GoogleTest/PointTests.cpp GoogleTest/MultiindexTests.cpp)
target_link_libraries(Google_Tests_run gtest gtest_main Multivariate_Polynomials)
