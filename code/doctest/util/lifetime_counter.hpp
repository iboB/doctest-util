// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "atomic_relaxed_counter.hpp"
#include <doctest/doctest.h>

namespace doctest
{
namespace util
{

struct lifetime_stats {
    int d_ctr{0}; // default construct
    int c_ctr{0}; // copy constructed
    int c_asgn{0}; // copy assigned
    int copies{0}; // total copies
    int m_ctr{0}; // move constructed
    int m_asgn{0}; // move assigned
    int living{0}; // total living
    int total{0}; // total constructed

    bool operator==(const lifetime_stats& other) const noexcept
    {
        return d_ctr == other.d_ctr
            && c_ctr == other.c_ctr
            && c_asgn == other.c_asgn
            && copies == other.copies
            && m_ctr == other.m_ctr
            && m_asgn == other.m_asgn
            && living == other.living
            && total == other.total;
    }
};

namespace impl
{
class basic_lifetime_stats
{
public:
    atomic_relaxed_counter<int> d_ctr{0}; // default construct
    atomic_relaxed_counter<int> c_ctr{0}; // copy constructed
    atomic_relaxed_counter<int> c_asgn{0}; // copy assigned
    atomic_relaxed_counter<int> copies{0}; // total copies
    atomic_relaxed_counter<int> m_ctr{0}; // move constructed
    atomic_relaxed_counter<int> m_asgn{0}; // move assigned
    atomic_relaxed_counter<int> living{0}; // total living
    atomic_relaxed_counter<int> total{0}; // total constructed

    template <typename F>
    void for_all(F&& f)
    {
        for (auto s = this; s; s = s->parent)
        {
            f(*s);
        }
    }

    lifetime_stats checkpoint() const noexcept
    {
        lifetime_stats ret;

        ret.d_ctr = d_ctr.load();
        ret.c_ctr = c_ctr.load();
        ret.c_asgn = c_asgn.load();
        ret.copies = copies.load();
        ret.m_ctr = m_ctr.load();
        ret.m_asgn = m_asgn.load();
        ret.living = living.load();
        ret.total = total.load();

        return ret;
    }
protected:
    basic_lifetime_stats* parent = nullptr;
};
}

template <typename Tag>
class lifetime_counter
{
public:
    struct lifetime_stats : public impl::basic_lifetime_stats
    {
        lifetime_stats() noexcept
        {
            parent = lifetime_counter::m_top_stats;
            lifetime_counter::m_top_stats = this;
        }

        ~lifetime_stats() noexcept
        {
            lifetime_counter::m_top_stats = parent;
        }
    };

    static const lifetime_stats& root_lifetime_stats() noexcept { return m_root_stats; }
    static const impl::basic_lifetime_stats& top_lifetime_stats() noexcept { return *m_top_stats; }

    lifetime_counter() noexcept
    {
        m_top_stats->for_all([](impl::basic_lifetime_stats& s) {
            ++s.d_ctr;
            ++s.living;
            ++s.total;
        });
    }

    lifetime_counter(const lifetime_counter&) noexcept
    {
        m_top_stats->for_all([](impl::basic_lifetime_stats& s) {
            ++s.c_ctr;
            ++s.copies;
            ++s.living;
            ++s.total;
        });
    }

    lifetime_counter& operator=(const lifetime_counter&) noexcept
    {
        m_top_stats->for_all([](impl::basic_lifetime_stats& s) {
            ++s.c_asgn;
            ++s.copies;
        });
        return *this;
    }

    lifetime_counter(lifetime_counter&&) noexcept
    {
        m_top_stats->for_all([](impl::basic_lifetime_stats& s) {
            ++s.m_ctr;
            ++s.living;
            ++s.total;
        });
    }

    lifetime_counter& operator=(lifetime_counter&&) noexcept
    {
        m_top_stats->for_all([](impl::basic_lifetime_stats& s) {
            ++s.m_asgn;
        });
        return *this;
    }
    ~lifetime_counter()
    {
        m_top_stats->for_all([](impl::basic_lifetime_stats& s) {
            --s.living;
        });
    }

private:
    static impl::basic_lifetime_stats* m_top_stats;
    static lifetime_stats m_root_stats;
};

template <typename T>
typename impl::basic_lifetime_stats* lifetime_counter<T>::m_top_stats;
template <typename T>
typename lifetime_counter<T>::lifetime_stats lifetime_counter<T>::m_root_stats;

struct lifetime_counter_sentry {
    const impl::basic_lifetime_stats& stats;
    lifetime_counter_sentry(const impl::basic_lifetime_stats& s) noexcept : stats(s) {}
    ~lifetime_counter_sentry() { CHECK(stats.living == 0); }
};



}
}
