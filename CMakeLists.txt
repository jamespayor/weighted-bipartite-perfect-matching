cmake_minimum_required(VERSION 3.9)
project(bipartite_perfect_matching)

set(CMAKE_CXX_STANDARD 11)

add_executable(bipartite_perfect_matching
        hungarian.cpp
        hungarian.h
        test.cpp)
