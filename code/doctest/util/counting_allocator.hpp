// doctest-util
// Copyright(c) 2022 Borislav Stanimirov
//
// SPDX-License-Identifier: MIT
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or https://opensource.org/licenses/MIT
//
#pragma once
#include <doctest/doctest.h>

#include <memory>

namespace doctest
{
namespace util
{

template <typename T>
class counting_allocator : private std::allocator<T>
{
    using super = std::allocator<T>;
public:
    size_t allocations = 0;
    size_t deallocations = 0;
    size_t allocated_bytes = 0;
    size_t deallocated_bytes = 0;

    counting_allocator() = default;

    // do nothing when copied
    // preserve original's values
    counting_allocator(const counting_allocator&) {}
    counting_allocator& operator=(const counting_allocator&) { return *this; }

    counting_allocator(counting_allocator&& other)
    {
        take_from(other);
    }
    counting_allocator& operator=(counting_allocator&& other)
    {
        take_from(other);
        return *this;
    }

    T* allocate(size_t n, const void* h = nullptr)
    {
        ++allocations;
        allocated_bytes += n * sizeof(T);
        return super::allocate(n);
    }

    void deallocate(T* p, size_t n)
    {
        ++deallocations;
        deallocated_bytes += n * sizeof(T);
        return super::deallocate(p, n);
    }

    ~counting_allocator()
    {
        CHECK(allocations == deallocations);
        CHECK(allocated_bytes == deallocated_bytes);
    }

private:
    void take_from(counting_allocator& other)
    {
        allocations = other.allocations;
        deallocations = other.deallocations;
        allocated_bytes = other.allocated_bytes;
        deallocated_bytes = other.deallocated_bytes;
        other.allocations = 0;
        other.deallocations = 0;
        other.allocated_bytes = 0;
        other.deallocated_bytes = 0;
    }
};

}
}
