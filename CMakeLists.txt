cmake_minimum_required(VERSION 3.16)
project(Multivariate_Polynomials)

set(CMAKE_CXX_STANDARD 14)

add_library(Multivariate_Polynomials point.cpp point.h Multiindex.cpp Multiindex.h)