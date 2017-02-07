// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_TUPLE_TRANSFORM_HPP
#define ALLOY_TUPLE_TRANSFORM_HPP

#include <alloy/config.hpp>
#include <alloy/lambda/curry.hpp>
#include <alloy/lambda/invoke.hpp>
#include <alloy/tuple/at.hpp>
#include <alloy/tuple/indices.hpp>
#include <alloy/tuple/tuple.hpp>

#include <alloy/detail/lookup.hpp>
#include <alloy/detail/object.hpp>
#include <alloy/detail/return.hpp>
#include <alloy/detail/traits.hpp>

#include <metal/list/list.hpp>
#include <metal/list/size.hpp>
#include <metal/value/same.hpp>

namespace alloy::detail {
    template<typename Clsr>
    struct transformer;

    template<typename Lbd, typename... Tpls>
    struct transformer<object<Lbd, Tpls...>>
    {
        using Clsr = object<Lbd, Tpls...>;
        Clsr&& clsr;

        template<typename Idx>
        constexpr auto operator ()(Idx&& idx) ALLOY_RETURN(
            invoke(
                ALLOY_LOOKUP(Lbd, Clsr&&, clsr),
                at(ALLOY_LOOKUP(Tpls, Clsr&&, clsr), static_cast<Idx&&>(idx))...
            )
        )
    };

    template<typename Lbd, typename Head, typename... Tail,
        where<(sizeof...(Tail) > 1)> = valid,
        where<metal::same<metal::size<props<Head>>, metal::size<props<Tail>>...>::value> = valid
    >
    constexpr auto transform(Lbd&& lbd, Head&& head, Tail&&... tail) ALLOY_RETURN(
        transform(
            transformer<object_t<Lbd&&, Head&&, Tail&&...>>{{
                static_cast<Lbd&&>(lbd),
                static_cast<Head&&>(head),
                static_cast<Tail&&>(tail)...
            }},
            indices(static_cast<Head&&>(head))
        )
    )

    template<typename Lbd, typename X, typename Y, typename... XPs, typename... YPs>
    constexpr auto transform_2(Lbd&& lbd, X&& x, Y&& y, metal::list<XPs...>, metal::list<YPs...>) ALLOY_RETURN(
        make_tuple(
            invoke(
                static_cast<Lbd&&>(lbd),
                ALLOY_LOOKUP(XPs, X&&, x),
                ALLOY_LOOKUP(YPs, Y&&, y)
            )...
        )
    )

    template<typename Lbd, typename X, typename Y,
        typename XPs = props<X>,
        typename YPs = props<Y>,
        where<metal::same<metal::size<XPs>, metal::size<YPs>>::value> = valid
    >
    constexpr auto transform(Lbd&& lbd, X&& x, Y&& y) ALLOY_RETURN(
        detail::transform_2(
            static_cast<Lbd&&>(lbd),
            static_cast<X&&>(x),
            static_cast<Y&&>(y),
            XPs{},
            YPs{}
        )
    )

    template<typename Lbd, typename X, typename... XPs>
    constexpr auto transform_1(Lbd&& lbd, X&& x, metal::list<XPs...>) ALLOY_RETURN(
        make_tuple(invoke(static_cast<Lbd&&>(lbd), ALLOY_LOOKUP(XPs, X&&, x))...)
    )

    template<typename Lbd, typename X,
        typename XPs = props<X>
    >
    constexpr auto transform(Lbd&& lbd, X&& x) ALLOY_RETURN(
        detail::transform_1(static_cast<Lbd&&>(lbd), static_cast<X&&>(x), XPs{})
    )

    inline constexpr struct {
        template<typename... Args>
        constexpr auto operator ()(Args&&... args) const ALLOY_RETURN(
            transform(static_cast<Args&&>(args)...)
        )
    } _transform{};
}

namespace alloy {
    inline constexpr auto transform = curry<2>(detail::_transform);
}

#endif
