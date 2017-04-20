// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#include <test.hpp>

template<typename M, typename N>
struct matrix {
    operator int() {
        static_assert(!alloy::is_invocable(alloy::reverse, val<M, N>()));
        static_assert(alloy::is_invocable(alloy::reverse, single<M, N>()));
        static_assert(alloy::is_invocable(alloy::reverse, many<M, iota<_0_, N>>()));

        static_assert(alloy::reverse(single<M, N>()) == single<M, N>());
        static_assert(alloy::reverse(many<M, iota<_0_, N>>()) == many<M, reverse<iota<_0_, N>>>());

        return 0;
    }
};

int main() {
    return test<matrix>;
}
