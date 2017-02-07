// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_TUPLE_APPLY_HPP
#define ALLOY_TUPLE_APPLY_HPP

#include <alloy/config.hpp>
#include <alloy/lambda/curry.hpp>
#include <alloy/lambda/invoke.hpp>

#include <alloy/detail/lookup.hpp>

#include <alloy/detail/traits.hpp>

#include <metal/list/list.hpp>

namespace alloy::detail {
    template<typename Lbd, typename Tpl, typename... Props>
    constexpr auto apply_impl(Lbd&& lbd, Tpl&& tpl, metal::list<Props...>) ALLOY_RETURN(
        invoke(static_cast<Lbd&&>(lbd), ALLOY_LOOKUP(Props, Tpl&&, tpl)...)
    )

    template<typename Lbd, typename Tpl,
        typename Props = props<Tpl>
    >
    constexpr auto apply(Lbd&& lbd, Tpl&& tpl) ALLOY_RETURN(
        apply_impl(
            static_cast<Lbd&&>(lbd),
            static_cast<Tpl&&>(tpl),
            Props{}
        )
    )

    constexpr inline struct {
        template<typename... Args>
        constexpr auto operator ()(Args&&... args) const ALLOY_RETURN(
            apply(static_cast<Args&&>(args)...)
        )
    } _apply{};
}

namespace alloy {
    inline constexpr auto apply = curry<2, 2>(detail::_apply);
}

#endif
