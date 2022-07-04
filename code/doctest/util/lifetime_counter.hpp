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

namespace impl
{
struct lifetime_stats
{
    int d_ctr = 0; // default construct
    int c_ctr = 0; // copy constructed
    int c_asgn = 0; // copy assigned
    int copies = 0; // total copies
    int m_ctr = 0; // move constructed
    int m_asgn = 0; // move assigned
    int living = 0; // total living

    lifetime_stats* parent = nullptr;

    template <typename F>
    void for_all(F&& f)
    {
        for (auto s = this; s; s = parent)
        {
            f(*s);
        }
    }
};
}

template <typename Tag>
class lifetime_counter
{
public:
    struct stats : public impl::lifetime_stats
    {
        stats()
        {
            parent = lifetime_counter::top;
            lifetime_counter::top = this;
        }

        ~stats()
        {
            lifetime_counter::top = parent;
        }
    };

    static stats* top;
    static stats bottom;

    lifetime_counter()
    {
        top->for_all([](impl::lifetime_stats& s) {
            ++s.d_ctr;
            ++s.living;
        });
    }

    lifetime_counter(const lifetime_counter&)
    {
        top->for_all([](impl::lifetime_stats& s) {
            ++s.c_ctr;
            ++s.copies;
            ++s.living;
        });
    }

    lifetime_counter& operator=(const lifetime_counter&)
    {
        top->for_all([](impl::lifetime_stats& s) {
            ++s.c_asgn;
            ++s.copies;
        });
    }

    lifetime_counter(lifetime_counter&&)
    {
        top->for_all([](impl::lifetime_stats& s) {
            ++s.m_ctr;
            ++s.living;
        });
    }

    lifetime_counter& operator=(lifetime_counter&&)
    {
        top->for_all([](impl::lifetime_stats& s) {
            ++s.m_asgn;
        });
    }
    ~lifetime_counter()
    {
        top->for_all([](impl::lifetime_stats& s) {
            --s.living;
        });
    }
};

template <typename T>
typename lifetime_counter<T>::stats* lifetime_counter<T>::top;
template <typename T>
typename lifetime_counter<T>::stats lifetime_counter<T>::bottom;

}
}
