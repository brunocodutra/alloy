// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_APPEND_HPP
#define ALLOY_APPEND_HPP

#include "config.hpp"
#include "detail.hpp"
#include "model.hpp"

namespace alloy {
    inline constexpr auto append = [](auto&&... ys) noexcept {
        return stream{
            [&ys...](auto&& snk) noexcept {
                return [&ys..., &snk](auto&&... xs) -> decltype(auto) {
                    return detail::invoke(
                        static_cast<decltype(snk)>(snk),
                        static_cast<decltype(xs)>(xs)...,
                        static_cast<decltype(ys)>(ys)...
                    );
                };
            }
        };
    };
}

#endif
