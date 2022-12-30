// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <atomic>
#include <type_traits>

namespace doctest {
namespace util {

// copied from https://github.com/iboB/itlib/blob/master/include/itlib/atomic.hpp
template <typename I>
struct atomic_relaxed_counter {
    static_assert(std::is_integral<I>::value, "counter must be integral");

    std::atomic<I> a;

    static const std::memory_order rel = std::memory_order_relaxed;

    atomic_relaxed_counter() noexcept = default;
    atomic_relaxed_counter(I init) noexcept : a(init) {};

    atomic_relaxed_counter(const atomic_relaxed_counter&) = delete;
    atomic_relaxed_counter& operator=(const atomic_relaxed_counter&) = delete;

    operator I() const noexcept {
        return a.load(rel);
    }
    I operator++() noexcept { return a.fetch_add(1, rel) + 1; }
    I operator++(int) noexcept { return a.fetch_add(1, rel); }
    I operator+=(I i) noexcept { return a.fetch_add(i, rel) + i; }
    I operator--() noexcept { return a.fetch_sub(1, rel) - 1; }
    I operator-(int) noexcept { return a.fetch_sub(1, rel); }
    I operator-=(I i) noexcept { return a.fetch_sub(i, rel) - i; }

    I operator=(I i) {
        a.store(i, rel);
        return i;
    }
};

}
}