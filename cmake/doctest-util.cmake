# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#

# add_doctest_test
#
# Creates an executable target, link with doctest, add
# as a test and add to a solution folder "test".
#
# Args:
#   * test - name of the test (visible when ctest is run)
#   * ...  - sources of the test
#
# Optional named args:
#   * SOURCES ... - explicitly specify sources
#   * LIBRARIES ... - specify additional librarires
#   * LABELS ... - specify test labels
#
# Notes:
# `add_doctest_test(mytest mytest.cpp)` will create:
#   * named test: mytest
#   * executable target: test-mytest
function(add_doctest_test test)
    cmake_parse_arguments(ARG "" "" "SOURCES;LIBRARIES;LABELS" ${ARGN})
    icm_add_test(
        NAME ${test}
        TARGET test-${test}
        LIBRARIES
            doctest::main
            doctest::util
            ${ARG_LIBRARIES}
        SOURCES
            ${ARG_UNPARSED_ARGUMENTS}
            ${ARG_SOURCES}
        LABELS ${ARG_LABELS}
        FOLDER test
    )
endfunction()

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
#   * LABELS ... - specify test labels
#
# Notes:
# `add_doctest_lib_test(core mylib test_core.cpp)` will create:
#   * named test: mylib-core
#   * executable target: test-mylib-core
function(add_doctest_lib_test test lib)
    cmake_parse_arguments(ARG "" "" "SOURCES;LIBRARIES;LABELS" ${ARGN})
    add_doctest_test(${lib}-${test}
        SOURCES
            ${ARG_SOURCES}
            ${ARG_UNPARSED_ARGUMENTS}
        LIBRARIES
            ${ARG_LIBRARIES}
            ${lib}
        LABELS ${ARG_LABELS}
    )
endfunction()
