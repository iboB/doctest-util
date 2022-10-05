# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(doctest-util)

# cpm
include(./get_cpm.cmake)

#######################################
# cmake lib
CPMAddPackage(gh:iboB/icm@1.4.1)

list(APPEND CMAKE_MODULE_PATH
    "${icm_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)

include(icm_dev_mode)
include(icm_testing)
include(doctest-util)

#######################################
# options

option(DOCTEST_UTIL_BUILD_TESTS "doctest-util: build tests" ${ICM_DEV_MODE})
mark_as_advanced(DOCTEST_UTIL_BUILD_TESTS)

#######################################
# packages
CPMAddPackage(gh:iboB/doctest-lib@2.4.9a)

#######################################
# c++

add_library(doctest-util INTERFACE)
add_library(doctest::util ALIAS doctest-util)
target_include_directories(doctest-util INTERFACE code)
target_link_libraries(doctest-util INTERFACE doctest::headers)

if(ICM_DEV_MODE)
    add_subdirectory(scratch)
endif()

if(DOCTEST_UTIL_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

