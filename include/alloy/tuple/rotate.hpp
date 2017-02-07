// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_TUPLE_ROTATE_HPP
#define ALLOY_TUPLE_ROTATE_HPP

#include <alloy/config.hpp>
#include <alloy/lambda/curry.hpp>

#include <alloy/detail/pick.hpp>
#include <alloy/detail/return.hpp>
#include <alloy/detail/traits.hpp>

#include <metal/list/rotate.hpp>
#include <metal/number/number.hpp>

namespace alloy::detail {
    template<typename Tpl, typename Idx,
        typename Props = metal::rotate<props<Tpl>, metal::number<value<Idx>>>
    >
    constexpr auto rotate(Tpl&& tpl, Idx&&) ALLOY_RETURN(
        pick(static_cast<Tpl&&>(tpl), Props{})
    )

    inline constexpr struct {
        template<typename... Args>
        constexpr auto operator ()(Args&&... args) const ALLOY_RETURN(
            rotate(static_cast<Args&&>(args)...)
        )
    } _rotate{};
}

namespace alloy {
    inline constexpr auto rotate = curry<2, 2>(detail::_rotate);
}

#endif
