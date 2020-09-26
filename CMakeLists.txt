cmake_minimum_required(VERSION 3.16)
project(Multivariate_Polynomials)

set(CMAKE_CXX_STANDARD 17)

# 'lib' is the folder with Google Test sources
add_subdirectory(GoogleTest/lib lib)
add_subdirectory(abseil-cpp)
find_library(abseil REQUIRED)
include_directories(include ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


add_library(Multivariate_Polynomials
        Polynomials/LagrangeSelector.cpp Polynomials/LagrangeSelector.h
        Polynomials/LinearInterpolationProblem.cpp Polynomials/LinearInterpolationProblem.h
        Polynomials/Multiindex.cpp Polynomials/Multiindex.h
        Geometry/point.cpp Geometry/point.h
        Polynomials/polynomial.cpp Polynomials/polynomial.h
        utilities/kbnsum.cpp utilities/kbnsum.h
        utilities/utilities.cpp utilities/utilities.h
        Polynomials/Lagrange.h
        Polynomials/GroebnerBasis.cpp Polynomials/GroebnerBasis.h
        Geometry/transform.cpp Geometry/transform.h)

# 'Google_Tests_run' is the target name
add_executable(Google_Tests_run
        GoogleTest/PointTests.cpp
        GoogleTest/MultiindexTests.cpp
        GoogleTest/PolynomialTests.cpp
        GoogleTest/LinearInterpolationProblemTests.cpp
        GoogleTest/GroebnerTest.cpp)

target_link_libraries(Google_Tests_run gtest gtest_main Multivariate_Polynomials absl::strings)
