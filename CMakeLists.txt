# doctest-util
# Copyright (c) 2022 Borislav Stanimirov
#
# SPDX-License-Identifier: MIT
# Distributed under the MIT Software License
# See accompanying file LICENSE.txt or https://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(doctest-util)

# cpm
include(./get_cpm.cmake)

#######################################
# cmake lib
CPMAddPackage(gh:iboB/icm@1.3.4)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${icm_SOURCE_DIR}
)

include(icm_dev_mode)
include(icm_testing)

#######################################
# options

option(DOCTEST_UTIL_BUILD_TESTS "doctest-util: build tests" ${ICM_DEV_MODE})
mark_as_advanced(DOCTEST_UTIL_BUILD_TESTS)

#######################################
# packages
CPMAddPackage(gh:iboB/doctest-lib@2.4.8a)

#######################################
# c++

add_library(doctest-util INTERFACE)
target_include_directories(doctest-util INTERFACE code)
target_link_libraries(doctest-util INTERFACE doctest)

if(ICM_DEV_MODE)
    add_subdirectory(scratch)
endif()

if(HUSE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

#######################################
# cmake

# add_doctest_lib_test
#
# Creates an executable target, link with doctest and a given lib, add
# as a test and add to a solution folder "test".
#
# Args:
#   * test - name of the test (visible when ctest is run)
#   * lib  - name of the library to test
#   * ...  - sources of the test
#
# Optional named args:
#   * SOURCES ... - explicitly specify sources
#   * LIBRARIES ... - specify additional librarires
#
# Notes:
# `add_doctest_lib_test(core mylib test_core.cpp)` will create:
#   * named test: mylib-core
#   * executable target: test-mylib-core
macro(add_doctest_lib_test test lib)
    cmake_parse_arguments(ARG "" "" "SOURCES;LIBRARIES" ${ARGN})
    icm_add_test(
        NAME ${lib}-${test}
        TARGET test-${lib}-${test}
        LIBRARIES
            doctest-main
            ${lib}
            ${ARG_LIBRARIES}
        SOURCES
            ${ARG_UNPARSED_ARGUMENTS}
            ${ARG_SOURCES}
        FOLDER test
    )
endmacro()
