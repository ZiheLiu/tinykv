cmake_minimum_required(VERSION 3.16)
project(tinykv)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

set(TINYKV_BUILD_TEST ON CACHE BOOL "Build test or not, default is true")

include_directories(".")

add_library(tinykv_lib
  "util/slice.h"
  "util/status.h"
  "util/status.cc")

add_executable(tinykv_run main.cpp)
add_executable(gen_data gen_data.cpp)

target_link_libraries(tinykv_run tinykv_lib)

if (TINYKV_BUILD_TEST)
    find_package(GTest REQUIRED)
    find_package(Threads REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
    
    add_executable(tinykv_test
      test/slice_test.cc
      test/status_test.cc
      test/all_tests.cc)
    
    target_link_libraries(tinykv_test
      tinykv_lib
      ${GTEST_BOTH_LIBRARIES}
      ${CMAKE_THREAD_LIBS_INIT})
    
    add_test(TinyKVTest tinykv_test)
    enable_testing()
endif (TINYKV_BUILD_TEST)
