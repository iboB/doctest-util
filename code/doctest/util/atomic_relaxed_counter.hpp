// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <atomic>
#include <type_traits>

namespace doctest {
namespace util {

template <typename I>
struct atomic_relaxed_counter {
    static const std::memory_order rel = std::memory_order_relaxed;

    atomic_relaxed_counter() noexcept = default;
    atomic_relaxed_counter(I init) noexcept : a(init) {};

    static_assert(std::is_integral<I>::value, "counter must be integral");
    std::atomic<I> a;
    operator I() const noexcept {
        return a.load(rel);
    }
    I operator++() { return a.fetch_add(1, rel) + 1; }
    I operator++(int) { return a.fetch_add(1, rel); }
    I operator+=(I i) { return a.fetch_add(i, rel) + i; }
    I operator--() { return a.fetch_sub(1, rel) - 1; }
    I operator-(int) { return a.fetch_sub(1, rel); }
    I operator-=(I i) { return a.fetch_sub(i, rel) - i; }
};

}
}