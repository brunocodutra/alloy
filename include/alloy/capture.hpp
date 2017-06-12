// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_CAPTURE_HPP
#define ALLOY_CAPTURE_HPP

#include "config.hpp"
#include "detail.hpp"
#include "model.hpp"

namespace alloy::detail {
    template<typename... Vs>
    constexpr decltype(auto) capture(Vs&&... vs) {
        return source{detail::args_t<Vs...>{static_cast<Vs&&>(vs)...}};
    }
}

namespace alloy {
    inline constexpr auto capture = [](auto&&... xs) {
        return detail::capture(static_cast<decltype(xs)>(xs)...);
    };
}

#endif
