cmake_minimum_required(VERSION 3.5)
project(tinykv)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

set(TINYKV_BUILD_TEST ON CACHE BOOL "Build test or not, default is true")

include_directories("." "include/")

add_library(tinykv "")
target_sources(tinykv
  PRIVATE
  include/constants.h
  util/cache.h
  util/cache.cc
  util/coding.h
  util/file.h
  util/file.cc
  util/hash.h
  util/hash.cc
  util/mutexlock.h
  util/random.h
  util/slice.h
  util/status.h
  util/status.cc
  util/zipf_query_generator.h
  util/zipf_query_generator.cc
  db/db.h
  db/db.cc
  db/hash_db.h
  db/hash_db.cc)

add_executable(query_data query_data.cc)
target_link_libraries(query_data tinykv)
target_link_libraries(query_data -lpthread)

add_executable(gen_data gen_data.cc)
target_link_libraries(gen_data tinykv)

if (TINYKV_BUILD_TEST)
    find_package(GTest REQUIRED)
    find_package(Threads REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
    
    add_executable(tinykv_test
      test/cache_test.cc
      test/slice_test.cc
      test/status_test.cc
      test/all_tests.cc)
    
    target_link_libraries(tinykv_test
      tinykv
      ${GTEST_BOTH_LIBRARIES}
      ${CMAKE_THREAD_LIBS_INIT})
    
    add_test(TinyKVTest tinykv_test)
    enable_testing()
endif (TINYKV_BUILD_TEST)
