// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_TUPLE_AT_HPP
#define ALLOY_TUPLE_AT_HPP

#include <alloy/config.hpp>
#include <alloy/lambda/curry.hpp>

#include <alloy/detail/return.hpp>
#include <alloy/detail/traits.hpp>

#include <metal/list/at.hpp>
#include <metal/number/number.hpp>

namespace alloy::detail {
    template<typename Tpl, typename Idx,
        typename Prop = metal::at<props<Tpl>, metal::number<value<Idx>>>
    >
    constexpr decltype(auto) at(Tpl&& tpl, Idx&&) noexcept {
        return ALLOY_LOOKUP(Prop, Tpl&&, tpl);
    }

    inline constexpr struct {
        template<typename... Args>
        constexpr auto operator ()(Args&&... args) const ALLOY_RETURN(
            at(static_cast<Args&&>(args)...)
        )
    } _at{};
}

namespace alloy {
    inline constexpr auto at = curry<2, 2>(detail::_at);
}

#endif
