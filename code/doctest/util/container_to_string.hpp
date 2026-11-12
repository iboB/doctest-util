// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//

// a specialization of StringMaker for container-like types (std::vector, std::list, etc.)
// include this header to enable it
#pragma once
#include <doctest/doctest.h>
#include <cstddef>

namespace doctest {

template <typename T>
concept ContainerLike =
    // not printable to ostream otherwise
    !requires(std::ostream & out, const T & c) {
        { out << c };
    } &&
    // has begin/end and size
    requires(const T& c) {
        { c.begin() };
        { c.end() };
        { c.size() };
    };

template <typename T> requires ContainerLike<T>
struct StringMaker<T> {
    static String convert(const T& container) {
        if (container.size() == 0) {
            return String("{}");
        }

        String ret = "{";
        size_t count = 0;
        for (const auto& elem : container) {
            if (count >= 16) {
                ret += ", [... ";
                ret += doctest::toString(container.size() - count);
                ret += " more]";
                break;
            }
            if (count > 0) {
                ret += ", ";
            }
            ret += doctest::toString(elem);
            ++count;
        }
        ret += "}";
        return ret;
    }
};

} // namespace doctest
