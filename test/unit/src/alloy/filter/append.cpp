// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#include <alloy.hpp>

#include "test.hpp"

template<auto X, auto Y, auto Z>
struct matrix {
    constexpr matrix() {
        decltype(auto) x = value<X, Y, Z>();
        decltype(auto) y = value<Z, X, Y>();
        decltype(auto) z = value<Y, Z, X>();

        static_assert(values<X, Y, Z>() >> alloy::append() >> expect(values<X, Y, Z>()));
        static_assert(values<X, Y, Z>() >> alloy::append(FWD(x)) >> expect(values<X, Y, Z>(), FWD(x)));
        static_assert(values<X, Y, Z>() >> alloy::append(FWD(x), FWD(y)) >> expect(values<X, Y, Z>(), FWD(x), FWD(y)));
        static_assert(values<X, Y, Z>() >> alloy::append(FWD(x), FWD(y), FWD(z)) >> expect(values<X, Y, Z>(), FWD(x), FWD(y), FWD(z)));
    }
};

int main() {
    return test<matrix>;
}
