// Copyright Bruno Dutra 2017-2018
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#include <alloy.hpp>

#include "test.hpp"

template<auto X, auto Y, auto Z>
struct matrix {
    constexpr matrix() {
        static_assert(values<X, Y, Z>() >> qualify<X>(alloy::sink{callable<X>(expect(values<X, Y, Z>()))}));
    }
};

int main() {
    return test<matrix>;
}
