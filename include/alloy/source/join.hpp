// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_SOURCE_JOIN_HPP
#define ALLOY_SOURCE_JOIN_HPP

#include "../config.hpp"
#include "../detail/invoke.hpp"
#include "../operators.hpp"
#include "../source/forward.hpp"
#include "../source/model.hpp"

namespace alloy::detail {
    template<typename Src>
    constexpr auto buffer(Src&& src) noexcept {
        return [&src](auto&& snk) noexcept {
            return [&src, &snk](auto&&... ys) -> decltype(auto) {
                return invoke(
                    static_cast<Src&&>(src), [&snk, &ys...](auto&&... xs) {
                        return invoke(static_cast<decltype(snk)>(snk),
                            static_cast<decltype(xs)>(xs)...,
                            static_cast<decltype(ys)>(ys)...);
                    });
            };
        };
    }
}

namespace alloy {
    inline constexpr auto join = [](auto&&... srcs) {
        return source{[&srcs...](auto&& snk) -> decltype(auto) {
            /* clang-format off */
            return (static_cast<decltype(snk)>(snk) << ... << filter{
                detail::buffer(static_cast<decltype(srcs)>(srcs))
            }) << forward();
            /* clang-format on */
        }};
    };
}

#endif
