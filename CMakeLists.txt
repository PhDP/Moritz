cmake_minimum_required(VERSION 2.8)
project(wagner)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type provided, default to RELEASE.")
  set(CMAKE_BUILD_TYPE "RELEASE")
endif ()

option(BuildTest  "BuildTest"  ON)
option(Sanitize   "Sanitize"   OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(Threads REQUIRED)
find_package(Boost REQUIRED)
find_package(Math)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  include(CheckCXXCompilerFlag)
  check_cxx_compiler_flag(-std=c++14 HAVE_FLAG_CXX_14)
  if (HAVE_FLAG_CXX_14)
    set(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_FLAGS}")
  else ()
    message(FATAL_ERROR "Gorram! Your compiler is too frakking old. C++11 support is required.")
  endif ()
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
  set(CMAKE_CXX_FLAGS_DEBUG "-g -pedantic -fno-omit-frame-pointer -fno-inline-functions")
endif ()

if (Sanitize)
  set(CMAKE_CXX_FLAGS "-fsanitize=address -fsanitize=undefined ${CMAKE_CXX_FLAGS}")
endif ()

# Set up directories
include_directories(. ${Boost_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)

if (BuildTest)
  enable_testing()
  add_subdirectory(test)
endif ()

message(STATUS "")
message(STATUS "WAGNER BUILD SUMMARY")
message(STATUS "  CMAKE_GENERATOR      : ${CMAKE_GENERATOR}")
message(STATUS "  Compiler ID          : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  Build type           : ${CMAKE_BUILD_TYPE}")
message(STATUS "  Build tests          : ${BuildTest}")
message(STATUS "  Sanitize flags       : ${Sanitize}")
message(STATUS "  Boost include dirs   : ${Boost_INCLUDE_DIRS}")
message(STATUS "  CXX_FLAGS            : ${CMAKE_CXX_FLAGS}")
if ("${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
  message(STATUS "  CXX_FLAGS_RELEASE    : ${CMAKE_CXX_FLAGS_RELEASE}")
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
  message(STATUS "  CXX_FLAGS_DEBUG      : ${CMAKE_CXX_FLAGS_DEBUG}")
endif ()
message(STATUS "")
