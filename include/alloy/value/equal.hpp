// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_VALUE_EQUAL_HPP
#define ALLOY_VALUE_EQUAL_HPP

#include <alloy/config.hpp>
#include <alloy/number/and.hpp>

#include <alloy/detail/return.hpp>

namespace alloy::detail {
    template<typename Head, typename... Tail>
    constexpr auto equal(Head&& head, Tail&&... tail) ALLOY_RETURN(
        and_(static_cast<Head&&>(head) == static_cast<Tail&&>(tail)...)
    )

    inline constexpr struct {
        template<typename... Args>
        constexpr auto operator ()(Args&&... args) const ALLOY_RETURN(
            equal(static_cast<Args&&>(args)...)
        )
    } _equal{};
}

namespace alloy {
    inline constexpr auto& equal = detail::_equal;
}

#endif
