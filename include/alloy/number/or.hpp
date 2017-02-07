// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_NUMBER_OR_HPP
#define ALLOY_NUMBER_OR_HPP

#include <alloy/config.hpp>
#include <alloy/number/number.hpp>

#include <alloy/detail/return.hpp>

namespace alloy::detail {
    template<typename... Xs>
    constexpr auto or_(Xs&&... xs) ALLOY_RETURN(
        (number<false>{} || ... || static_cast<Xs&&>(xs))
    )

    inline constexpr struct {
        template<typename... Xs>
        constexpr auto operator ()(Xs&&... xs) const ALLOY_RETURN(
            or_(static_cast<Xs&&>(xs)...)
        )
    } _or_{};
}

namespace alloy {
    inline constexpr auto& or_ = detail::_or_;
}

#endif
