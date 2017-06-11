// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_FORWARD_HPP
#define ALLOY_FORWARD_HPP

#include "config.hpp"
#include "detail.hpp"
#include "model.hpp"

namespace alloy {
    template<typename... Vs>
    constexpr decltype(auto) forward(Vs&&... vs) noexcept {
        return source{detail::args_t<Vs&&...>{static_cast<Vs&&>(vs)...}};
    }
}

#endif
