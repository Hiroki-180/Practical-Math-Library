cmake_minimum_required(VERSION 3.11)

project(Utility CXX)


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
  Utility
  STATIC
  CSVParser.cpp
  ../../include/PML/Utility/CSVParser.h)