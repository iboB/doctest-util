macro(doctes_util_test test)
    add_doctest_lib_test(${test} doctest-util ${ARGN} LABELS unit-test)
endmacro()

doctes_util_test(lifetime_counter t-lifetime_counter.cpp)
doctes_util_test(counting_allocator t-counting_allocator.cpp)
