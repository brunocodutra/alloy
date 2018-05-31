#include <alloy.hpp>

#include "test.hpp"

template<auto X, auto Y, auto Z>
struct matrix {
    constexpr matrix() {

        auto f = [](auto&& x, auto&& y) -> decltype(auto) {
            return FWD(x) - FWD(y);
        };

        static_assert(values<X, Y, Z>() >> alloy::foldr(qualify<Z>(callable<Z>(f)), value<X, Y, 0>()) >> expect(value<X, Y, (Z % 2) ? (Z / 2) : (-Z / 2)>()));
    }
};

int main() {
    return test<matrix>;
}
