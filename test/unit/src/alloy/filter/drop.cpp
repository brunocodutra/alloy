// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#include <alloy.hpp>

#include "test.hpp"

template<auto X, auto Y, auto Z>
struct matrix {
    constexpr matrix() {
        decltype(auto) z = qualify<Z>(instance<alloy::constant<Z>>);

        constexpr auto given = cat(values<X, Y, Z>(), values<Z, X, Y>());

        static_assert(given >> alloy::drop(Z) >> expect(values<Z, X, Y>()));
        static_assert(given >> alloy::drop(FWD(z)) >> expect(values<Z, X, Y>()));
    }
};

int main() {
    return test<matrix>;
}
