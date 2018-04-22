cmake_minimum_required(VERSION 3.11)

project(Math CXX)

include_directories(../../include)

set(CMAKE_CXX_STANDARD 17)

#Clang or AppleClang
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
 set(CMAKE_CXX_FLAGS_RELEASE "-fno-aligned-allocation -msse3 -mssse3 -msse4.1 -msse4.2 -mavx -mavx2")
 set(CMAKE_CXX_FLAGS_DEBUG   "-fno-aligned-allocation -msse3 -mssse3 -msse4.1 -msse4.2 -mavx -mavx2")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
 #GCC
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
 #Intel Compiler
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
 #Microsoft Visual C++
 set(CMAKE_CXX_FLAGS_RELEASE "/MT  /W4")
 set(CMAKE_CXX_FLAGS_DEBUG   "/MTd /W4")
endif()

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