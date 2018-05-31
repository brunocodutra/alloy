#ifndef ALLOY_STREAM_APPEND_HPP
#define ALLOY_STREAM_APPEND_HPP

#include "../config.hpp"
#include "../detail.hpp"
#include "../stream/model.hpp"

namespace alloy {
    inline constexpr auto append = [](auto&&... ys) noexcept {
        stream impl = [&ys...](auto&& snk) noexcept {
            return [&ys..., &snk](auto&&... xs) -> decltype(auto) {
                return detail::invoke(static_cast<decltype(snk)>(snk),
                    static_cast<decltype(xs)>(xs)...,
                    static_cast<decltype(ys)>(ys)...);
            };
        };

        return impl;
    };
}

#endif
