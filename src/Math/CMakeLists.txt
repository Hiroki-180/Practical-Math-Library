cmake_minimum_required(VERSION 3.11)

project(Math CXX)


#####################
## Compile Options ##
#####################

set(CMAKE_CXX_STANDARD 14)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  #Clang or AppleClang
  set(CMAKE_CXX_FLAGS "-Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -fno-aligned-allocation -msse3 -mssse3 -msse4.1 -msse4.2 -mavx -mavx2")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  #GCC
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  #Intel Compiler
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  #Microsoft Visual C++
  set(CMAKE_CXX_FLAGS         "/W4")
  set(CMAKE_CXX_FLAGS_RELEASE "/MT  /DNDEBUG")
  set(CMAKE_CXX_FLAGS_DEBUG   "/MTd")
endif()


#####################
####### Links #######
#####################

include_directories(../../include)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON) 

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../../)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../../)

add_library(
  Math
  STATIC
  numeric_simd/accumulate.cpp
  numeric_simd/adjacent_divide.cpp
  numeric_simd/inner_prod.cpp
  numeric_simd/positive_difference.cpp
  pmath.cpp
  ../../include/PML/Math/numeric_simd.h
  ../../include/PML/Math/constants.h
  ../../include/PML/Math/pmath.h
  ../../include/PML/Math/numeric_simd/accumulate.h
  ../../include/PML/Math/numeric_simd/adjacent_divide.h
  ../../include/PML/Math/numeric_simd/inner_prod.h
  ../../include/PML/Math/numeric_simd/positive_difference.h
  ../../include/PML/Math/pmath.h)

SOURCE_GROUP("Header files\\numeric_simd" FILES ../../include/PML/Math/numeric_simd/accumulate.h)
SOURCE_GROUP("Header files\\numeric_simd" FILES ../../include/PML/Math/numeric_simd/adjacent_divide.h)
SOURCE_GROUP("Header files\\numeric_simd" FILES ../../include/PML/Math/numeric_simd/inner_prod.h)
SOURCE_GROUP("Header files\\numeric_simd" FILES ../../include/PML/Math/numeric_simd/positive_difference.h)

SOURCE_GROUP("Source files\\numeric_simd" FILES numeric_simd/accumulate.cpp)
SOURCE_GROUP("Source files\\numeric_simd" FILES numeric_simd/adjacent_divide.cpp)
SOURCE_GROUP("Source files\\numeric_simd" FILES numeric_simd/inner_prod.cpp)
SOURCE_GROUP("Source files\\numeric_simd" FILES numeric_simd/positive_difference.cpp)