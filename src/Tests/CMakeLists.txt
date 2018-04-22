cmake_minimum_required(VERSION 3.10)

project(Tests CXX)

add_dependencies(Core Math)

include_directories(../../include)
include_directories(./)

link_directories(../../lib/googletest)

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
 TestMath/TestPMath.cpp)

set(
 LINK_LIBRARIES_GTEST
 optimized gtest
 debug gtestd)

set(
 LINK_LIBRARIES_GTEST_MAIN
 optimized gtest_main
 debug gtest_maind)

target_link_libraries(
 Tests ${LINK_LIBRARIES_GTEST} ${LINK_LIBRARIES_GTEST_MAIN} Core Math)