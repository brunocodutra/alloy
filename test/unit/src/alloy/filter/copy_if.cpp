// Copyright Bruno Dutra 2017-2018
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#include <alloy.hpp>
#include <algorithm>

#include "test.hpp"

template<auto X, auto Y, auto Z>
struct matrix {
    constexpr matrix() {
        auto f = [](auto&& x) {
            return FWD(x) < alloy::constant<X>{};
        };

        auto F = [](auto&& x) {
            return FWD(x) < X;
        };

        constexpr auto given = cat(values<X, Y, Z>(), values<Z, X, Y>());
        constexpr auto expected = expect(values<X, Y, std::min(Z, X)>(), values<Z, X, std::min(Y, X)>());

        static_assert(given >> alloy::copy_if(qualify<Z>(callable<Z>(f))) >> expected);
        static_assert(given >> alloy::copy_if(qualify<Z>(callable<Z>(F))) >> expected);
    }
};

int main() {
    return test<matrix>;
}
