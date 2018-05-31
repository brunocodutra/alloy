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
