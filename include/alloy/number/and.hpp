// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_NUMBER_AND_HPP
#define ALLOY_NUMBER_AND_HPP

#include <alloy/config.hpp>
#include <alloy/number/number.hpp>

#include <alloy/detail/return.hpp>

namespace alloy::detail {
    template<typename... Xs>
    constexpr auto and_(Xs&&... xs) ALLOY_RETURN(
        (number<true>{} && ... && static_cast<Xs&&>(xs))
    )

    inline constexpr struct {
        template<typename... Xs>
        constexpr auto operator ()(Xs&&... xs) const ALLOY_RETURN(
            and_(static_cast<Xs&&>(xs)...)
        )
    } _and_{};
}

namespace alloy {
    inline constexpr auto& and_ = detail::_and_;
}

#endif
