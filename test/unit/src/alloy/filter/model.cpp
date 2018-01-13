// Copyright Bruno Dutra 2017-2018
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#include <alloy.hpp>

#include "test.hpp"

template<auto X, auto Y, auto Z>
struct matrix {
    constexpr matrix() {
        auto f = [](auto&& sink) {
            return [&sink](auto&&... args) -> decltype(auto) {
                return FWD(sink)(FWD(args)..., FWD(args)...);
            };
        };

        {
            constexpr auto expected = expect(values<X, Y, Z>(), values<X, Y, Z>());

            static_assert((values<X, Y, Z>() >> qualify<X>(alloy::filter{callable<X>(f)})) >> expected);
            static_assert(values<X, Y, Z>() >> (qualify<X>(alloy::filter{callable<X>(f)}) >> expected));
        }

        {
            constexpr auto expected = expect(values<X, Y, Z>(), values<X, Y, Z>(), values<X, Y, Z>(), values<X, Y, Z>());

            static_assert(((values<X, Y, Z>() >> qualify<X>(alloy::filter{callable<X>(f)})) >> qualify<Y>(alloy::filter{callable<Y>(f)})) >> expected);
            static_assert((values<X, Y, Z>() >> qualify<X>(alloy::filter{callable<X>(f)})) >> (qualify<Y>(alloy::filter{callable<Y>(f)}) >> expected));
            static_assert(values<X, Y, Z>() >> (qualify<X>(alloy::filter{callable<X>(f)}) >> (qualify<Y>(alloy::filter{callable<Y>(f)}) >> expected)));
        }
    }
};

int main() {
    return test<matrix>;
}
