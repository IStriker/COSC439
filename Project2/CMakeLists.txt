cmake_minimum_required(VERSION 3.7)
project(Project2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES library.cpp library.h)
add_library(Project2 ${SOURCE_FILES})