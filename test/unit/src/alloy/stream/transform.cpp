#include <alloy.hpp>

#include "test.hpp"

template<auto X, auto Y, auto Z>
struct matrix {
    constexpr matrix() {

        auto f = [](auto&& x) -> decltype(auto) {
            return -FWD(x);
        };

        static_assert(values<X, Y, Z>() >> alloy::transform(qualify<Z>(callable<Z>(f))) >> expect(values<X, Y, -Z, 1 - Z>()));
    }
};

int main() {
    return test<matrix>;
}
