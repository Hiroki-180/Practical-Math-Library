cmake_minimum_required(VERSION 3.11)

project(Math CXX)

include_directories(../../include)

set(CMAKE_CXX_STANDARD 14)

if(MSVC)
 set(CMAKE_CXX_FLAGS_RELEASE "/MT /W4")
 set(CMAKE_CXX_FLAGS_DEBUG "/MTd /W4")
else(MSVC)
 set(CMAKE_CXX_FLAGS_RELEASE "/W4")
 set(CMAKE_CXX_FLAGS_DEBUG "/W4") 
endif(MSVC)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../../)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../../)

add_library(
 Math
 STATIC
 numeric/accumulate.cpp
 numeric/adjacent_divide.cpp
 numeric/inner_prod.cpp
 numeric/positive_difference.cpp
 pmath.cpp
 ../../include/PML/Math/numeric/accumulate.h
 ../../include/PML/Math/numeric/adjacent_divide.h
 ../../include/PML/Math/numeric/inner_prod.h
 ../../include/PML/Math/numeric/positive_difference.h
 ../../include/PML/Math/pmath.h)