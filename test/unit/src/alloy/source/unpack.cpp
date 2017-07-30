// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#include <alloy.hpp>

#include "test.hpp"

template<auto X, auto Y, auto Z>
struct matrix {
    constexpr matrix() {
        using variant = std::variant<
            std::decay_t<decltype(value<X, Y, Z>())>,
            std::decay_t<decltype(value<Z, X, Y>())>,
            std::decay_t<decltype(value<Y, Z, X>())>
        >;

        auto v0 = [](){return variant(std::in_place_index<0>, copy(value<X, Y, Z>()));};
        auto v1 = [](){return variant(std::in_place_index<1>, copy(value<Z, X, Y>()));};
        auto v2 = [](){return variant(std::in_place_index<2>, copy(value<Y, Z, X>()));};

        static_assert(alloy::unpack(qualify<X>(v0())) >> expect(std::get<0>(qualify<X>(v0()))));
        static_assert(alloy::unpack(qualify<X>(v1())) >> expect(std::get<1>(qualify<X>(v1()))));
        static_assert(alloy::unpack(qualify<X>(v2())) >> expect(std::get<2>(qualify<X>(v2()))));

        auto t0 = [](){return std::make_tuple();};
        auto t1 = [](){return std::make_tuple(value<X, Y, Z>());};
        auto t2 = [](){return std::make_tuple(value<X, Y, Z>(), value<Z, X, Y>());};
        auto t3 = [](){return std::make_tuple(value<X, Y, Z>(), value<Z, X, Y>(), value<Y, Z, X>());};

        static_assert(alloy::unpack() >> expect());
        static_assert(alloy::unpack(qualify<X>(t0())) >> expect());
        static_assert(alloy::unpack(qualify<X>(t0()), qualify<Y>(t0())) >> expect());
        static_assert(alloy::unpack(qualify<X>(t0()), qualify<Y>(t0()), qualify<Z>(t0())) >> expect());

        static_assert(alloy::unpack(qualify<X>(t1()))
            >> expect(std::get<0>(qualify<X>(t1()))));

        static_assert(alloy::unpack(qualify<X>(t2()))
            >> expect(std::get<0>(qualify<X>(t2())), std::get<1>(qualify<X>(t2()))));

        static_assert(alloy::unpack(qualify<X>(t3()))
            >> expect(std::get<0>(qualify<X>(t3())), std::get<1>(qualify<X>(t3())), std::get<2>(qualify<X>(t3()))));

        static_assert(alloy::unpack(qualify<X>(t1()), qualify<Y>(v1()), qualify<Z>(t1()))
            >> expect(std::get<0>(qualify<X>(t1())), std::get<1>(qualify<Y>(v1())), std::get<0>(qualify<Z>(t1()))));

        static_assert(alloy::unpack(qualify<X>(v0()), qualify<Y>(t1()), qualify<Z>(v2()))
            >> expect(std::get<0>(qualify<X>(v0())), std::get<0>(qualify<Y>(t1())), std::get<2>(qualify<Z>(v2()))));
    }
};

int main() {
    return test<matrix>;
}
