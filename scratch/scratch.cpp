// doctest-util
// Copyright(c) 2022 Borislav Stanimirov
//
// SPDX-License-Identifier: MIT
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or https://opensource.org/licenses/MIT
//

#include <doctest/util/lifetime_counter.hpp>
#include <iostream>

int main()
{
    using doctest::util::lifetime_counter;

    lifetime_counter<int> lc;

    auto lc2 = lc;
}
