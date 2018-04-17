#include <alloy.hpp>

#include "test.hpp"

template<auto X, auto Y, auto Z>
struct matrix {
    constexpr matrix() {
        decltype(auto) x = values<X, Y, Z>();
        decltype(auto) y = values<Z, X, Y>();
        decltype(auto) z = values<Y, Z, X>();

        static_assert(alloy::join() >> expect());
        static_assert(alloy::join(FWD(x)) >> expect(FWD(x)));
        static_assert(alloy::join(FWD(x), FWD(y)) >> expect(FWD(x), FWD(y)));
        static_assert(alloy::join(FWD(x), FWD(y), FWD(z)) >> expect(FWD(x), FWD(y), FWD(z)));
    }
};

int main() {
    return test<matrix>;
}
