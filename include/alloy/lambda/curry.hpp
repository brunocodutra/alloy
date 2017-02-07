// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_LAMBDA_CURRY_HPP
#define ALLOY_LAMBDA_CURRY_HPP

#include <alloy/config.hpp>
#include <alloy/lambda/invoke.hpp>

#include <alloy/detail/lookup.hpp>
#include <alloy/detail/object.hpp>
#include <alloy/detail/return.hpp>
#include <alloy/detail/traits.hpp>

#include <cstdint>

namespace alloy::detail {
    template<auto M, auto N, typename Clsr>
    struct curried;

    template<auto M = 0U, auto N = UINTMAX_MAX, typename Lbd, typename... Args,
        where<sizeof...(Args) < M && M <= N> = valid
    >
    constexpr decltype(auto) curry(Lbd&& lbd, Args&&... args) noexcept {
        return curried<M, N, object_t<Lbd&&, Args&&...>>{
            static_cast<Lbd&&>(lbd),
            static_cast<Args&&>(args)...
        };
    }

    template<auto M, auto N, typename Lbd_, typename... Args_>
    struct curried<M, N, object<Lbd_, Args_...>> {
        using Clsr = object<Lbd_, Args_...>;

        Clsr clsr;

        curried() = delete;
        curried(curried const&) = delete;

        curried(curried&&) = default;

        constexpr curried(type<Lbd_>&& lbd, type<Args_>&&... args) noexcept
            : clsr(
                static_cast<type<Lbd_>&&>(lbd),
                static_cast<type<Args_>&&>(args)...
            )
        {}

        template<typename... Args,
            where<(sizeof...(Args) > 0)> = valid,
            where<(sizeof...(Args_) + sizeof...(Args) < M)> = valid
        >
        constexpr decltype(auto) operator ()(Args&&... args) noexcept {
            return curry<M, N>(
                ALLOY_LOOKUP(Lbd_, Clsr&&, clsr),
                ALLOY_LOOKUP(Args_, Clsr&&, clsr)...,
                static_cast<Args&&>(args)...
            );
        }

        template<typename... Args,
            where<(sizeof...(Args) > 0)> = valid,
            where<(sizeof...(Args_) + sizeof...(Args) < M)> = valid
        >
        constexpr decltype(auto) operator ()(Args&&... args) const noexcept {
            return curry<M, N>(
                ALLOY_LOOKUP(Lbd_, Clsr const&&, clsr),
                ALLOY_LOOKUP(Args_, Clsr const&&, clsr)...,
                static_cast<Args&&>(args)...
            );
        }

        template<typename... Args,
            where<(sizeof...(Args_) + sizeof...(Args) >= M)> = valid,
            where<(sizeof...(Args_) + sizeof...(Args) <= N)> = valid
        >
        constexpr auto operator ()(Args&&... args) ALLOY_RETURN(
            invoke(
                ALLOY_LOOKUP(Lbd_, Clsr&&, clsr),
                ALLOY_LOOKUP(Args_, Clsr&&, clsr)...,
                static_cast<Args&&>(args)...
            )
        )

        template<typename... Args,
            where<(sizeof...(Args_) + sizeof...(Args) >= M)> = valid,
            where<(sizeof...(Args_) + sizeof...(Args) <= N)> = valid
        >
        constexpr auto operator ()(Args&&... args) const ALLOY_RETURN(
            invoke(
                ALLOY_LOOKUP(Lbd_, Clsr const&&, clsr),
                ALLOY_LOOKUP(Args_, Clsr const&&, clsr)...,
                static_cast<Args&&>(args)...
            )
        )
    };

    template<auto M = 0U, auto N = UINTMAX_MAX>
    struct _curry_t {
        template<typename... Args>
        constexpr auto operator ()(Args&&... args) const ALLOY_RETURN(
            curry<M, N>(static_cast<Args&&>(args)...)
        )
    };

    template<auto M = 0U, auto N = UINTMAX_MAX>
    constexpr _curry_t<M, N> _curry{};
}

namespace alloy {
    template<auto M = 0U, auto N = UINTMAX_MAX>
    constexpr auto& curry = detail::_curry<M, N>;
}

#endif
