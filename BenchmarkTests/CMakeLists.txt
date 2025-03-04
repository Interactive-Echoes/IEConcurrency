# SPDX-License-Identifier: GPL-2.0-only
# Copyright © Interactive Echoes. All rights reserved.
# Author: mozahzah

cmake_minimum_required(VERSION 3.20)
project(IEConcurrencyBenchmarkTests VERSION 1.0.0 LANGUAGES CXX)

message("\n------------------------------------------------------------")
message("${PROJECT_NAME}\n")

set(Benchmark_SOURCE_FILES 
  "./SPSCQueueBenchmark.cpp"
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
foreach(SOURCE_FILE ${Benchmark_SOURCE_FILES})
  get_filename_component(file_name ${SOURCE_FILE} NAME_WE)
  message(STATUS "Setting up executable for ${file_name}")
  add_executable(${file_name} ${SOURCE_FILE})
  target_link_libraries(${file_name} PUBLIC IEConcurrency)
  target_link_libraries(${file_name} PUBLIC benchmark)
  target_link_libraries(${file_name} PUBLIC Boost::lockfree)
  find_library(GoogleProfiler NAMES profiler)
  if (GoogleProfiler)
    message(STATUS "Found Google Profiler library: ${GoogleProfiler}")
    get_filename_component(GoogleProfiler_DIR ${GoogleProfiler} DIRECTORY)
    target_link_directories(${file_name} PUBLIC ${GoogleProfiler_DIR})
    target_link_libraries(${file_name} PUBLIC profiler)
  endif()

  set_target_properties(${file_name} PROPERTIES 
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endforeach()

message("------------------------------------------------------------\n")