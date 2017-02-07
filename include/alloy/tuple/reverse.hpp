// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_TUPLE_REVERSE_HPP
#define ALLOY_TUPLE_REVERSE_HPP

#include <alloy/config.hpp>

#include <alloy/detail/pick.hpp>
#include <alloy/detail/return.hpp>

#include <metal/list/reverse.hpp>

namespace alloy::detail {
    template<typename Tpl,
        typename Props = metal::reverse<props<Tpl>>
    >
    constexpr auto reverse(Tpl&& tpl) ALLOY_RETURN(
        pick(static_cast<Tpl&&>(tpl), Props{})
    )

    inline constexpr struct {
        template<typename... Args>
        constexpr auto operator ()(Args&&... args) const ALLOY_RETURN(
            reverse(static_cast<Args&&>(args)...)
        )
    } _reverse{};
}

namespace alloy {
    inline constexpr auto& reverse = detail::_reverse;
}

#endif
