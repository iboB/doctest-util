// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <doctest/doctest.h>
#include <cstddef>
#include <concepts>

namespace doctest {

template <typename T>
concept ContainerLike = requires(T c) {
    { c.begin() } -> std::same_as<typename T::iterator>;
    { c.end() } -> std::same_as<typename T::iterator>;
    { c.size() } -> std::integral;
};

template <typename T> requires ContainerLike<T>
struct StringMaker<T> {
    static String convert(const T& container) {
        if (container.size() == 0) {
            return String("{}");
        }

        String ret = "{";
        int count = 0;
        for (const auto& elem : container) {
            if (count > 0) {
                ret += ", ";
            }
            ret += doctest::toString(elem);
            ++count;
            if (count >= 16) {
                ret += ", [... ";
                ret += doctest::toString(container.size() - count);
                ret += " more]";
                break;
            }
        }
        ret += "}";
        return ret;
    }
};

} // namespace doctest
