#include <alloy.hpp>

#include "test.hpp"

template<auto X, auto Y, auto Z>
struct matrix {
    constexpr matrix() {
        decltype(auto) x = qualify<X>(instance<alloy::constant<X>>);
        decltype(auto) y = qualify<X>(instance<alloy::constant<Y>>);
        decltype(auto) z = qualify<X>(instance<alloy::constant<Z>>);

        static_assert(values<X, Y, INF>() >> alloy::at() >> expect());

        static_assert(values<X, Y, INF>() >> alloy::at(X) >> expect(value<X, Y, X>()));
        static_assert(values<X, Y, INF>() >> alloy::at(FWD(x)) >> expect(value<X, Y, X>()));

        static_assert(values<X, Y, INF>() >> alloy::at(X, Y) >> expect(value<X, Y, X>(), value<X, Y, Y>()));
        static_assert(values<X, Y, INF>() >> alloy::at(FWD(x), FWD(y)) >> expect(value<X, Y, X>(), value<X, Y, Y>()));

        static_assert(values<X, Y, INF>() >> alloy::at(X, Y, Z) >> expect(value<X, Y, X>(), value<X, Y, Y>(), value<X, Y, Z>()));
        static_assert(values<X, Y, INF>() >> alloy::at(FWD(x), FWD(y), FWD(z)) >> expect(value<X, Y, X>(), value<X, Y, Y>(), value<X, Y, Z>()));
    }
};

int main() {
    return test<matrix>;
}
