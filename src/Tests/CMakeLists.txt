cmake_minimum_required(VERSION 3.10)

project(Tests CXX)

add_dependencies(Core Math Utility)

include_directories(../../include)
include_directories(./)


if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
 # Clang
 # ToDo
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
 # AppleClang
 link_directories(../../lib/googletest/AppleClang_9.1.0)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
 # GCC
 # ToDo
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
 # Intel Compiler
 # ToDo
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
 # Microsoft Visual C++
 link_directories(../../lib/googletest/MSVC_2017_Community)
endif()

set(CMAKE_CXX_STANDARD 14)

if(MSVC)
 set(CMAKE_CXX_FLAGS_RELEASE "/MT /W4 /DNDEBUG")
 set(CMAKE_CXX_FLAGS_DEBUG "/MTd /W4")
endif(MSVC)

add_executable(
 Tests
 stdafx.h
 stdafx.cpp
 targetver.h
 TestCore/TestCore.cpp
 TestMath/TestConstants.cpp
 TestMath/TestNumericSIMD.cpp
 TestMath/TestPMath.cpp
 TestUtility/TestCSVParser.cpp)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
 # Clang
 # ToDo
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
 # AppleClang
 set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3")
 set(
  LINK_LIBRARIES_GTEST
  optimized libgtest.a
  debug libgtestd.a)
 set(
  LINK_LIBRARIES_GTEST_MAIN
  optimized libgtest_main.a
  debug libgtest_maind.a)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
 # GCC
 # ToDo
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
 # Intel Compiler
 # ToDo
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
 # Microsoft Visual C++
 set(
  LINK_LIBRARIES_GTEST
  optimized gtest
  debug gtestd)
 set(
  LINK_LIBRARIES_GTEST_MAIN
  optimized gtest_main
  debug gtest_maind)
endif()

target_link_libraries(
 Tests ${LINK_LIBRARIES_GTEST} ${LINK_LIBRARIES_GTEST_MAIN} Core Math Utility)
