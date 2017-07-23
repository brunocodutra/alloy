// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_OPERATORS_HPP
#define ALLOY_OPERATORS_HPP

#include "config.hpp"
#include "detail/invoke.hpp"
#include "detail/traits.hpp"
#include "filter/model.hpp"
#include "sink/model.hpp"
#include "source/model.hpp"

namespace alloy::detail {
    template<typename F,
        typename G,
        requires<instanceof <F, filter>> = valid,
        requires<instanceof <G, filter>> = valid>
    constexpr decltype(auto) operator<<(F&& f, G&& g) {
        return filter{[&f, &g](auto&& snk) -> decltype(auto) {
            return static_cast<decltype(snk)>(snk)
                << static_cast<F&&>(f) << static_cast<G&&>(g);
        }};
    }

    template<typename F,
        typename Src,
        requires<instanceof <F, filter>> = valid,
        requires<! instanceof <Src, filter>> = valid>
    constexpr decltype(auto) operator<<(F&& f, Src&& src) {
        return source{[&f, &src](auto&& snk) -> decltype(auto) {
            return static_cast<decltype(snk)>(snk)
                << static_cast<F&&>(f) << static_cast<Src&&>(src);
        }};
    }

    template<typename Snk,
        typename F,
        requires<! instanceof <Snk, filter>> = valid,
        requires<instanceof <F, filter>> = valid>
    constexpr decltype(auto) operator<<(Snk&& snk, F&& f) {
        return sink{invoke(static_cast<F&&>(f), static_cast<Snk&&>(snk))};
    }

    template<typename Snk,
        typename Src,
        requires<! instanceof <Snk, filter>> = valid,
        requires<! instanceof <Src, filter>> = valid,
        requires<instanceof <Snk, sink> || instanceof <Src, source>> = valid>
    constexpr decltype(auto) operator<<(Snk&& snk, Src&& src) {
        return invoke(static_cast<Src&&>(src), static_cast<Snk&&>(snk));
    }
}

#endif
