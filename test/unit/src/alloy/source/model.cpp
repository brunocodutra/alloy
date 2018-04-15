#include <alloy.hpp>

#include "test.hpp"

template<auto X, auto Y, auto Z>
struct matrix {
    constexpr matrix() {
        auto f = [](auto&& sink) {
            return values<X, Y, Z>()(FWD(sink));
        };

        static_assert(qualify<X>(alloy::source{callable<X>(f)}) >> expect(values<X, Y, Z>()));
    }
};

int main() {
    return test<matrix>;
}
