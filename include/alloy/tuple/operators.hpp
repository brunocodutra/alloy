// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_TUPLE_OPERATORS_HPP
#define ALLOY_TUPLE_OPERATORS_HPP

#include <alloy/config.hpp>
#include <alloy/number/and.hpp>
#include <alloy/tuple/apply.hpp>
#include <alloy/tuple/transform.hpp>
#include <alloy/tuple/tuple.hpp>
#include <alloy/value/equal.hpp>

#include <alloy/detail/return.hpp>
#include <alloy/detail/traits.hpp>

#include <metal/list/size.hpp>

namespace alloy::detail {
    template<typename X, typename Y,
        where<metal::size<props<X>>{} == metal::size<props<Y>>{}> = valid
    >
    constexpr auto operator ==(X&& x, Y&& y) ALLOY_RETURN(
        apply(_and_, transform(_equal, static_cast<X&&>(x), static_cast<Y&&>(y)))
    )

    template<typename X, typename Y,
        where<metal::size<props<X>>{} != metal::size<props<Y>>{}> = valid
    >
    constexpr auto operator ==(X&&, Y&&) ALLOY_RETURN(
        number<false>{}
    )
}

#endif
