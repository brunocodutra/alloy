// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_TUPLE_ACCUMULATE_HPP
#define ALLOY_TUPLE_ACCUMULATE_HPP

#include <alloy/config.hpp>
#include <alloy/lambda/curry.hpp>
#include <alloy/lambda/invoke.hpp>
#include <alloy/tuple/apply.hpp>
#include <alloy/tuple/at.hpp>
#include <alloy/tuple/indices.hpp>
#include <alloy/value/fold.hpp>

#include <alloy/detail/lookup.hpp>
#include <alloy/detail/object.hpp>
#include <alloy/detail/return.hpp>
#include <alloy/detail/traits.hpp>

#include <metal/list/size.hpp>
#include <metal/value/same.hpp>

namespace alloy::detail {
    template<typename Clsr>
    struct accumulator;

    template<typename Lbd, typename... Tpls>
    struct accumulator<object<Lbd, Tpls...>>
    {
        using Clsr = object<Lbd, Tpls...>;
        Clsr&& clsr;

        template<typename State, typename Idx>
        constexpr auto operator ()(State&& state, Idx&& idx) ALLOY_RETURN(
            invoke(
                ALLOY_LOOKUP(Lbd, Clsr&&, clsr),
                static_cast<State&&>(state),
                at(ALLOY_LOOKUP(Tpls, Clsr&&, clsr), static_cast<Idx&&>(idx))...
            )
        )
    };

    template<typename Lbd, typename State, typename Head, typename... Tail,
       where<sizeof...(Tail)> = valid,
       where<metal::same<metal::size<props<Head>>, metal::size<props<Tail>>...>::value> = valid
    >
    constexpr auto accumulate(Lbd&& lbd, State&& state, Head&& head, Tail&&... tail) ALLOY_RETURN(
        accumulate(
            accumulator<object_t<Lbd&&, Head&&, Tail&&...>>{{
                static_cast<Lbd&&>(lbd),
                static_cast<Head&&>(head),
                static_cast<Tail&&>(tail)...
            }},
            static_cast<State&&>(state),
            indices(static_cast<Head&&>(head))
        )
    )

    template<typename Lbd, typename State, typename Tpl>
    constexpr auto accumulate(Lbd&& lbd, State&& state, Tpl&& tpl) ALLOY_RETURN(
        apply(
            curry<3>(_foldl)(
                static_cast<Lbd&&>(lbd),
                static_cast<State&&>(state)
            ),
            static_cast<Tpl&&>(tpl)
        )
    )

    inline constexpr struct {
        template<typename... Args>
        constexpr auto operator ()(Args&&... args) const ALLOY_RETURN(
            accumulate(static_cast<Args&&>(args)...)
        )
    } _accumulate{};
}

namespace alloy {
    inline constexpr auto accumulate = curry<3>(detail::_accumulate);
}

#endif
