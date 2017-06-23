// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_STREAM_PREPEND_HPP
#define ALLOY_STREAM_PREPEND_HPP

#include "../config.hpp"
#include "../detail/invoke.hpp"
#include "../stream/model.hpp"

/* clang-format off */
namespace alloy {
    inline constexpr auto prepend = [](auto&&... xs) noexcept {
        return stream{
            [&xs...](auto&& snk) noexcept {
                return [&xs..., &snk](auto&&... ys) -> decltype(auto) {
                    return detail::invoke(static_cast<decltype(snk)>(snk),
                        static_cast<decltype(xs)>(xs)...,
                        static_cast<decltype(ys)>(ys)...);
                };
            }
        };
    };
}
/* clang-format off */

#endif