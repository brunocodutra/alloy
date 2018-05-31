#include <alloy.hpp>

#include "test.hpp"

template<auto X, auto Y, auto Z>
struct matrix {
    constexpr matrix() {
        decltype(auto) z = qualify<Z>(instance<alloy::constant<Z>>);

        constexpr auto given = cat(values<X, Y, Z>(), values<Z, X, Y>());

        static_assert(given >> alloy::take(Z) >> expect(values<X, Y, Z>()));
        static_assert(given >> alloy::take(FWD(z)) >> expect(values<X, Y, Z>()));
    }
};

int main() {
    return test<matrix>;
}
