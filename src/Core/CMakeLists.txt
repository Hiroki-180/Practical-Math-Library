cmake_minimum_required(VERSION 3.11)

project(Core CXX)

include_directories(../../include)

set(CMAKE_CXX_STANDARD 14)

if(MSVC)
 set(CMAKE_CXX_FLAGS_RELEASE "/MT /W4")
 set(CMAKE_CXX_FLAGS_DEBUG "/MTd /W4")
endif(MSVC)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../../)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../../)

add_library(
 Core
 STATIC
 CPUDispatcher.cpp
 ../../include/PML/Core/aligned_array.h
 ../../include/PML/Core/aligned_vector.h
 ../../include/PML/Core/CPUDispatcher.h)