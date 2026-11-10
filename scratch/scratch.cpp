// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <doctest/util/container_to_string.hpp>
#include <doctest/doctest.h>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <string_view>

TEST_CASE("scratch") {
    std::vector<int> a = {1, 2, 3, 4, 5};
    std::vector<int> b = {1, 4, 3, 4, 5};

    CHECK(a == b);

    std::string astr = "hello";
    CHECK(astr == "helo");

    std::string_view bstr = "world";
    CHECK(bstr == "wrld");
}

int main(int argc, char** argv) {
    return doctest::Context(argc, argv).run();
}
