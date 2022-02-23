cmake_minimum_required(VERSION 3.16)
project(Multivariate_Polynomials)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(ABSL_DIR /usr/local)
set(ABSL_PROPAGATE_CXX_STD ON)
add_subdirectory(abseil-cpp)

include_directories(${ABSL_DIR}/include ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


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
        Geometry/transform.cpp Geometry/transform.h
        Geometry/ConvexHull2D.cpp Geometry/ConvexHull2D.h
        utilities/analysis.cpp utilities/analysis.h)

# 'Google_Tests_run' is the target name
add_executable(Google_Tests_run
        test/PointTests.cpp
        test/MultiindexTests.cpp
        test/PolynomialTests.cpp
        test/LinearInterpolationProblemTests.cpp
        test/GroebnerTest.cpp
        test/ConvexHullTest.cpp
        test/analysis_test.cpp)

target_link_libraries(Google_Tests_run gtest gtest_main Multivariate_Polynomials
        absl::strings absl::random_random)

include(GoogleTest)
gtest_discover_tests(Google_Tests_run)
