# SPDX-License-Identifier: GPL-2.0-only
# Copyright © Interactive Echoes. All rights reserved.
# Author: mozahzah

cmake_minimum_required(VERSION 3.20)
project(IEConcurrencyExamples VERSION 1.0.0 LANGUAGES CXX)

message("\n------------------------------------------------------------")
message("${PROJECT_NAME}\n")

file(GLOB Example_SOURCE_FILES "./*.cpp")
foreach(SOURCE_FILE ${Example_SOURCE_FILES})
  get_filename_component(file_name ${SOURCE_FILE} NAME_WE)
  message(STATUS "Setting up executable for ${file_name}")
  add_executable(${file_name} ${SOURCE_FILE})
  target_link_libraries(${file_name} PUBLIC IEConcurrency)

  set_target_properties(${file_name} PROPERTIES 
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endforeach()

message("------------------------------------------------------------\n")