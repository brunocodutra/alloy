#ifndef ALLOY_FILTER_APPEND_HPP
#define ALLOY_FILTER_APPEND_HPP

#include "../config.hpp"
#include "../detail.hpp"
#include "../filter/model.hpp"

/* clang-format off */
namespace alloy {
    inline constexpr auto append = [](auto&&... ys) noexcept {
        return filter{
            [&ys...](auto&& snk) noexcept {
                return [&ys..., &snk](auto&&... xs) -> decltype(auto) {
                    return detail::invoke(static_cast<decltype(snk)>(snk),
                        static_cast<decltype(xs)>(xs)...,
                        static_cast<decltype(ys)>(ys)...);
                };
            }
        };
    };
}
/* clang-format on */

#endif
