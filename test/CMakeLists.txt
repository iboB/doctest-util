macro(doctes_util_test test)
    add_doctest_lib_test(${test} doctest-util ${ARGN})
endmacro()

doctes_util_test(lifetime_counter t-lifetime_counter.cpp)
