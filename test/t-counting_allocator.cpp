#include <doctest/util/counting_allocator.hpp>

#include <vector>
#include <string>

template <typename T>
using cvec = std::vector<T, doctest::util::counting_allocator<T>>;

using cstr = std::basic_string<char, std::char_traits<char>, doctest::util::counting_allocator<char>>;

TEST_CASE("counting allocator simple")
{
    cvec<int> ivec;
    ivec.reserve(10);
    for (int i = 0; i < 40; ++i) ivec.push_back(i);

    cvec<int> ivec2 = ivec;
    ivec2.clear();
    ivec2.resize(50);

    //cvec<int> ivec3 = std::move(ivec);
}