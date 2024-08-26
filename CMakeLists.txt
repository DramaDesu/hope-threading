cmake_minimum_required(VERSION 3.11)

project(hope-thread-main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_SAMPLES "Build samples" ON)

if(BUILD_TESTS)
	enable_testing()
	add_subdirectory(test)
endif()

if(BUILD_SAMPLES)
	add_subdirectory(samples/sync)
	add_subdirectory(samples/hash_map_perf_test)
endif()

add_subdirectory(lib)