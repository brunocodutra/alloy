// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_OPERATORS_HPP
#define ALLOY_OPERATORS_HPP

#include "config.hpp"
#include "detail/invoke.hpp"
#include "detail/traits.hpp"
#include "source/model.hpp"
#include "sink/model.hpp"
#include "stream/model.hpp"

namespace alloy::detail {
    template<typename S, typename T,
        requires<instanceof<S, stream>> = valid,
        requires<instanceof<T, stream>> = valid
    >
    constexpr decltype(auto) operator <<(S&& s, T&& t) {
        return stream{
            [&s, &t](auto&& snk) -> decltype(auto) {
                return static_cast<decltype(snk)>(snk)
                    << static_cast<S&&>(s)
                    << static_cast<T&&>(t);
            }
        };
    }

    template<typename Stm, typename Src,
        requires<instanceof<Stm, stream>> = valid,
        requires<!instanceof<Src, stream>> = valid
    >
    constexpr decltype(auto) operator <<(Stm&& stm, Src&& src) {
        return source{
            [&stm, &src](auto&& snk) -> decltype(auto) {
                return static_cast<decltype(snk)>(snk)
                    << static_cast<Stm&&>(stm)
                    << static_cast<Src&&>(src);
            }
        };
    }

    template<typename Snk, typename Stm,
        requires<!instanceof<Snk, stream>> = valid,
        requires<instanceof<Stm, stream>> = valid
    >
    constexpr decltype(auto) operator <<(Snk&& snk, Stm&& stm) {
        return sink{invoke(static_cast<Stm&&>(stm), static_cast<Snk&&>(snk))};
    }

    template<typename Snk, typename Src,
        requires<!instanceof<Snk, stream>> = valid,
        requires<!instanceof<Src, stream>> = valid,
        requires<instanceof<Snk, sink> || instanceof<Src, source>> = valid
    >
    constexpr decltype(auto) operator <<(Snk&& snk, Src&& src) {
        return invoke(static_cast<Src&&>(src), static_cast<Snk&&>(snk));
    }
}

#endif
