// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_VALUE_FOLD_HPP
#define ALLOY_VALUE_FOLD_HPP

#include <alloy/config.hpp>
#include <alloy/lambda/curry.hpp>
#include <alloy/lambda/invoke.hpp>
#include <alloy/tuple/indices.hpp>

#include <alloy/detail/return.hpp>
#include <alloy/detail/traits.hpp>

#include <metal/value/same.hpp>

namespace alloy::detail {
    struct nil {};

    template<typename Lbd, typename Arg>
    struct folder {
        Lbd&& lbd;
        Arg&& arg;
    };

    template<typename Lbd, typename Arg>
    constexpr decltype(auto) operator >>=(nil, folder<Lbd, Arg> ref) noexcept {
        return static_cast<Arg&&>(ref.arg);
    }

    template<typename Lbd, typename Arg, typename Stt,
        where<!metal::same<strip<Stt>, nil>::value> = valid
    >
    constexpr auto operator >>=(Stt&& stt, folder<Lbd, Arg> ref) ALLOY_RETURN(
        invoke(
            static_cast<Lbd&&>(ref.lbd),
            static_cast<Stt&&>(stt),
            static_cast<Arg&&>(ref.arg)
        )
    )

    template<typename Lbd, typename... Args,
        where<(sizeof...(Args) > 0)> = valid
    >
    constexpr auto foldl(Lbd&& lbd, Args&&... args) ALLOY_RETURN(
        (nil{} >>= ... >>= folder<Lbd, Args>{
            static_cast<Lbd&&>(lbd),
            static_cast<Args&&>(args)
        })
    )

    inline constexpr struct {
        template<typename... Args>
        constexpr auto operator ()(Args&&... args) const ALLOY_RETURN(
            foldl(static_cast<Args&&>(args)...)
        )
    } _foldl{};

    template<typename Lbd, typename Arg>
    constexpr decltype(auto) operator <<=(folder<Lbd, Arg> ref, nil) noexcept {
        return static_cast<Arg&&>(ref.arg);
    }

    template<typename Lbd, typename Arg, typename Stt,
        where<!metal::same<strip<Stt>, nil>::value> = valid
    >
    constexpr auto operator <<=(folder<Lbd, Arg> ref, Stt&& stt) ALLOY_RETURN(
        invoke(
            static_cast<Lbd&&>(ref.lbd),
            static_cast<Arg&&>(ref.arg),
            static_cast<Stt&&>(stt)
        )
    )

    template<typename Lbd, typename... Args,
        where<(sizeof...(Args) > 0)> = valid
    >
    constexpr auto foldr(Lbd&& lbd, Args&&... args) ALLOY_RETURN(
        (folder<Lbd, Args>{
            static_cast<Lbd&&>(lbd),
            static_cast<Args&&>(args)
        } <<= ... <<= nil{})
    )

    inline constexpr struct {
        template<typename... Args>
        constexpr auto operator ()(Args&&... args) const ALLOY_RETURN(
            foldr(static_cast<Args&&>(args)...)
        )
    } _foldr{};
}

namespace alloy {
    inline constexpr auto foldl = curry<2>(detail::_foldl);
    inline constexpr auto foldr = curry<2>(detail::_foldr);
}
#endif
