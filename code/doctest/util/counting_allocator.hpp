// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <doctest/doctest.h>

#include <memory>
#include <type_traits>

namespace doctest
{
namespace util
{

class counting_allocator_payload
{
public:
    counting_allocator_payload() = default;
    counting_allocator_payload(const counting_allocator_payload&) = delete;
    counting_allocator_payload& operator=(const counting_allocator_payload&) = delete;
    counting_allocator_payload(counting_allocator_payload&&) = delete;
    counting_allocator_payload& operator=(counting_allocator_payload&&) = delete;

    ~counting_allocator_payload()
    {
        CHECK(allocations == deallocations);
        CHECK(allocated_bytes == deallocated_bytes);
    }

    size_t allocations = 0;
    size_t deallocations = 0;
    size_t allocated_bytes = 0;
    size_t deallocated_bytes = 0;
};

template <typename T, template <typename> class Super>
class basic_counting_allocator : private Super<T>
{
    // never null this pointer is never null except in moved-out allocators
    // such should be inaccessible anyway
    std::shared_ptr<counting_allocator_payload> m_payload;
public:
    using super_type = Super<T>;
    using typename super_type::value_type;

    template <typename... Args, typename std::enable_if<std::is_constructible<super_type, Args...>::value, int>::type = 0>
    basic_counting_allocator(Args&&... args)
        : super_type(std::forward<Args>(args)...)
        , m_payload(std::make_shared<counting_allocator_payload>())
    {}

    // explicit casts
    super_type& super() { return *this; }
    const super_type& super() const { return *this; }

    basic_counting_allocator(const basic_counting_allocator&) = default;
    basic_counting_allocator& operator=(const basic_counting_allocator&) = default;

    // intentionally don't define these
    // always copy
    // counting_allocator(counting_allocator&& other) noexcept;
    // counting_allocator& operator=(counting_allocator&& other) noexcept;

    // rebind
    template <typename U>
    basic_counting_allocator(const basic_counting_allocator<U, Super>& other)
        : super_type(other.super())
        , m_payload(other.payload_ptr())
    {}

    const counting_allocator_payload& payload() const { return *m_payload; }
    const std::shared_ptr<counting_allocator_payload>& payload_ptr() const { return m_payload; }

    T* allocate(size_t n, const void* = nullptr)
    {
        auto& pl = *m_payload;
        ++pl.allocations;
        pl.allocated_bytes += n * sizeof(T);
        return super_type::allocate(n);
    }

    void deallocate(T* p, size_t n)
    {
        auto& pl = *m_payload;
        ++pl.deallocations;
        pl.deallocated_bytes += n * sizeof(T);
        return super_type::deallocate(p, n);
    }
};

template <typename T>
struct counting_allocator : public basic_counting_allocator<T, std::allocator>
{
    using basic_counting_allocator<T, std::allocator>::basic_counting_allocator;
};

}
}
