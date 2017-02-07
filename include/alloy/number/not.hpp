// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_NUMBER_NOT_HPP
#define ALLOY_NUMBER_NOT_HPP

#include <alloy/config.hpp>

#include <alloy/detail/return.hpp>

namespace alloy::detail {
    template<typename X>
    constexpr auto not_(X&& x) ALLOY_RETURN(
        !static_cast<X&&>(x)
    )

    inline constexpr struct {
        template<typename... Xs>
        constexpr auto operator ()(Xs&&... xs) const ALLOY_RETURN(
            not_(static_cast<Xs&&>(xs)...)
        )
    } _not_{};
}

namespace alloy {
    inline constexpr auto& not_ = detail::_not_;
}

#endif
