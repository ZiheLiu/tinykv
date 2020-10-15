cmake_minimum_required(VERSION 3.16)
project(tinykv)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

set(TINYKV_BUILD_TEST ON CACHE BOOL "Build test or not, default is true")

include_directories(".")

aux_source_directory(util util_source_dir)

add_executable(tinykv_run
  ${util_source_dir}
  main.cc )
add_executable(gen_data
  ${util_source_dir}
  gen_data.cc)

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
