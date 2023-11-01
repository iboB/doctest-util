// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <doctest/util/counting_allocator.hpp>

#include <vector>
#include <string>

using doctest::util::counting_allocator;
using doctest::util::basic_counting_allocator;

TEST_CASE("bare")
{
    counting_allocator<int> a;
    auto p = a.allocate(2);
    CHECK(a.payload().allocations == 1);
    CHECK(a.payload().deallocations == 0);
    CHECK(a.payload().allocated_bytes == 2 * sizeof(int));
    CHECK(a.payload().deallocated_bytes == 0);

    auto b = a;
    CHECK(b.payload_ptr() == a.payload_ptr());
    b.deallocate(p, 2);

    auto c = std::move(b);
    CHECK(c.payload_ptr() == a.payload_ptr());
}

template <typename T>
using cvec = std::vector<T, counting_allocator<T>>;
using cstr = std::basic_string<char, std::char_traits<char>, counting_allocator<char>>;

TEST_CASE("counting allocator simple")
{
    cvec<int> ivec;
    ivec.reserve(10);
    for (int i = 0; i < 40; ++i) ivec.push_back(i);

    cvec<int> ivec2 = ivec;
    ivec2.clear();
    ivec2.resize(50);

    cvec<int> ivec3 = std::move(ivec);

    cvec<cstr> svec;
    svec.push_back("x");
    svec.push_back("aaaaaaaaaaaa/bbbbbbbbbb/ccccccccccc");
    svec.push_back("ddddddddddd/eeeeeeeee/ffffffffffff");
}

#if (__cplusplus >= 201700)

#include <memory_resource>

template <typename T>
struct pmr_counting_allocator : public basic_counting_allocator<T, std::pmr::polymorphic_allocator>
{
    using basic_counting_allocator<T, std::pmr::polymorphic_allocator>::basic_counting_allocator;
};

template <typename T>
using pmvec = std::vector<T, pmr_counting_allocator<T>>;

TEST_CASE("pmr")
{
    pmvec<int> ivec;
    ivec.reserve(10);
    for (int i = 0; i < 20; ++i) ivec.push_back(i);
}

#endif
