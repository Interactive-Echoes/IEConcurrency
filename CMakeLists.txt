# SPDX-License-Identifier: GPL-2.0-only
# Copyright © Interactive Echoes. All rights reserved.
# Author: mozahzah

cmake_minimum_required(VERSION 3.20)
project(IEConcurrency VERSION 1.0.0 LANGUAGES CXX)

message("\n------------------------------------------------------------")
message("${PROJECT_NAME}\n")

message("Compiler Setup")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
message("Using Compiler: ${CMAKE_CXX_COMPILER_ID}")
message("version: ${CMAKE_CXX_COMPILER_VERSION}")
message("CXX standard: ${CMAKE_CXX_STANDARD}")
message("Working environment")
message("System name: ${CMAKE_SYSTEM_NAME}")
message("System version: ${CMAKE_SYSTEM_VERSION}")
message("System processor: ${CMAKE_SYSTEM_PROCESSOR}")

message("Gathering source files and creating Library")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(IEConcurrency INTERFACE)
target_include_directories(IEConcurrency INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(IEConcurrency INTERFACE "-Wno-interference-size")

message("Setting up install")
install(TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
include(InstallRequiredSystemLibraries)

message("Adding extra directories")
add_subdirectory(Examples)

message("------------------------------------------------------------\n")