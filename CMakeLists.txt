cmake_minimum_required(VERSION 3.5)
project(tinykv)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

set(TINYKV_BUILD_TEST ON CACHE BOOL "Build test or not, default is true")

include_directories("." "include/")

aux_source_directory(util util_source_dir)

add_executable(query_data
  ${util_source_dir}
  include/options.h
  include/constants.h
  util/hash.h
  util/hash.cc
  util/coding.h
  util/zipf_query_generator.cc
  util/zipf_query_generator.h
  db/db.h
  db/db.cc
  db/hash_db.h
  db/hash_db.cc
  query_data.cc)
add_executable(gen_data
  ${util_source_dir}
  include/constants.h
  gen_data.cc)

target_link_libraries(query_data -lpthread)

if (TINYKV_BUILD_TEST)
    find_package(GTest REQUIRED)
    find_package(Threads REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
    
    aux_source_directory(test test_source_dir)
    
    add_executable(tinykv_test
      ${util_source_dir}
      ${test_source_dir})
    
    target_link_libraries(tinykv_test
      ${GTEST_BOTH_LIBRARIES}
      ${CMAKE_THREAD_LIBS_INIT})
    
    add_test(TinyKVTest tinykv_test)
    enable_testing()
endif (TINYKV_BUILD_TEST)
