#include <alloy.hpp>

#include "test.hpp"

template<auto X, auto Y, auto Z>
struct matrix {
    constexpr matrix() {

        auto f = [](auto&& x, auto&& y) -> decltype(auto) {
            return FWD(x) - FWD(y);
        };

        static_assert(values<X, Y, Z>() >> alloy::foldl(qualify<Z>(callable<Z>(f)), value<X, Y, 0>()) >> expect(value<X, Y, - Z * (Z - 1) / 2>()));
    }
};

int main() {
    return test<matrix>;
}
