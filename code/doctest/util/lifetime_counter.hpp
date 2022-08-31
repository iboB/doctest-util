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
class lifetime_stats
{
public:
    int d_ctr = 0; // default construct
    int c_ctr = 0; // copy constructed
    int c_asgn = 0; // copy assigned
    int copies = 0; // total copies
    int m_ctr = 0; // move constructed
    int m_asgn = 0; // move assigned
    int living = 0; // total living

    template <typename F>
    void for_all(F&& f)
    {
        for (auto s = this; s; s = s->parent)
        {
            f(*s);
        }
    }

    bool operator==(const lifetime_stats& other) const
    {
        return d_ctr == other.d_ctr
            && c_ctr == other.c_ctr
            && c_asgn == other.c_asgn
            && copies == other.copies
            && m_ctr == other.m_ctr
            && m_ctr == other.m_asgn
            && living == other.living;
    }
protected:
    lifetime_stats* parent = nullptr;
};
}

template <typename Tag>
class lifetime_counter
{
public:
    struct lifetime_stats : public impl::lifetime_stats
    {
        lifetime_stats()
        {
            parent = lifetime_counter::m_top_stats;
            lifetime_counter::m_top_stats = this;
        }

        ~lifetime_stats()
        {
            lifetime_counter::m_top_stats = parent;
        }
    };

    static const lifetime_stats& root_lifetime_stats() { return m_root_stats; }
    static const impl::lifetime_stats& top_lifetime_stats() { return *m_top_stats; }

    lifetime_counter()
    {
        m_top_stats->for_all([](impl::lifetime_stats& s) {
            ++s.d_ctr;
            ++s.living;
        });
    }

    lifetime_counter(const lifetime_counter&)
    {
        m_top_stats->for_all([](impl::lifetime_stats& s) {
            ++s.c_ctr;
            ++s.copies;
            ++s.living;
        });
    }

    lifetime_counter& operator=(const lifetime_counter&)
    {
        m_top_stats->for_all([](impl::lifetime_stats& s) {
            ++s.c_asgn;
            ++s.copies;
        });
        return *this;
    }

    lifetime_counter(lifetime_counter&&)
    {
        m_top_stats->for_all([](impl::lifetime_stats& s) {
            ++s.m_ctr;
            ++s.living;
        });
    }

    lifetime_counter& operator=(lifetime_counter&&)
    {
        m_top_stats->for_all([](impl::lifetime_stats& s) {
            ++s.m_asgn;
        });
        return *this;
    }
    ~lifetime_counter()
    {
        m_top_stats->for_all([](impl::lifetime_stats& s) {
            --s.living;
        });
    }

private:
    static impl::lifetime_stats* m_top_stats;
    static lifetime_stats m_root_stats;
};

template <typename T>
typename impl::lifetime_stats* lifetime_counter<T>::m_top_stats;
template <typename T>
typename lifetime_counter<T>::lifetime_stats lifetime_counter<T>::m_root_stats;

struct lifetime_counter_sentry {
    const impl::lifetime_stats& stats;
    lifetime_counter_sentry(const impl::lifetime_stats& s) : stats(s) {}
    ~lifetime_counter_sentry() { CHECK(stats.living == 0); }
};

}
}
