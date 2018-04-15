#include <alloy.hpp>

#include "test.hpp"

template<auto X, auto Y, auto Z>
struct matrix {
    constexpr matrix() {
        static_assert(values<X, Y, Z>() >> qualify<X>(alloy::sink{callable<X>(expect(values<X, Y, Z>()))}));
    }
};

int main() {
    return test<matrix>;
}
