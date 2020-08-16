#ifndef ALLOY_OPERATORS_HPP
#define ALLOY_OPERATORS_HPP

#include "config.hpp"
#include "detail.hpp"
#include "sink/model.hpp"
#include "source/model.hpp"
#include "stream/model.hpp"

namespace alloy::detail {
template <typename F,
    typename G,
    requires<instanceof <F, stream>> = valid,
    requires<instanceof <G, stream>> = valid>
constexpr decltype(auto) operator>>(F&& f, G&& g) noexcept
{
    return stream { [&f, &g](auto&& snk) -> decltype(auto) {
        return static_cast<F&&>(f)
            >> (static_cast<G&&>(g) >> static_cast<decltype(snk)>(snk));
    } };
}

template <typename Str,
    typename Src,
    requires<! instanceof <Src, stream>> = valid,
    requires<instanceof <Str, stream>> = valid>
constexpr decltype(auto) operator>>(Src&& src, Str&& str) noexcept
{
    return source { [&src, &str](auto&& snk) -> decltype(auto) {
        return static_cast<Src&&>(src)
            >> (static_cast<Str&&>(str) >> static_cast<decltype(snk)>(snk));
    } };
}

template <typename Snk,
    typename Str,
    requires<instanceof <Str, stream>> = valid,
    requires<! instanceof <Snk, stream>> = valid>
constexpr decltype(auto) operator>>(Str&& str, Snk&& snk)
{
    return sink { invoke(static_cast<Str&&>(str), static_cast<Snk&&>(snk)) };
}

template <typename Snk,
    typename Src,
    requires<! instanceof <Src, stream>> = valid,
    requires<! instanceof <Snk, stream>> = valid,
    requires<instanceof <Src, source> || instanceof <Snk, sink>> = valid>
constexpr decltype(auto) operator>>(Src&& src, Snk&& snk)
{
    return invoke(static_cast<Src&&>(src), static_cast<Snk&&>(snk));
}
}

#endif
