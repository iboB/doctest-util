// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//

#include <doctest/util/lifetime_counter.hpp>
#include <iostream>

int main()
{
    using doctest::util::lifetime_counter;

    lifetime_counter<int> lc;

    auto lc2 = lc;
}
