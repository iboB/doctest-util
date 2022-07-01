// doctest-util
// Copyright(c) 2022 Borislav Stanimirov
//
// SPDX-License-Identifier: MIT
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or https://opensource.org/licenses/MIT
//
#pragma once
#include <doctest/doctest.h>

namespace doctest
{
namespace util
{

template <typename Tag>
class lifetime_counter
{
public:
    struct stats
    {
        int d_ctr = 0; // default construct
        int copies = 0; // total copies
        int c_ctr = 0; // copy constructed
        int c_asgn = 0; // copy assigned
        int m_ctr = 0; // move constructed
        int m_asgn = 0; // move assigned
        int living = 0; // total living

        stats* parent = nullptr;

        ~stats()
        {
        }
    };

    static stats* top;
    static stats bottom;

    lifetime_counter() {
    }
    lifetime_counter(const lifetime_counter& other) {
    }
    lifetime_counter& operator=(const lifetime_counter& other) {
    }
    lifetime_counter(lifetime_counter&& other) {
    }
    lifetime_counter& operator=(lifetime_counter&& other) {
    }
    ~lifetime_counter() {
    }
};

}
}
