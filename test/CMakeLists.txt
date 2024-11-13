# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
macro(doctest_util_test test)
    add_doctest_lib_test(${test} doctest-util t-${test}.cpp LABELS unit-test)
endmacro()

doctest_util_test(lifetime_counter)
doctest_util_test(counting_allocator)
