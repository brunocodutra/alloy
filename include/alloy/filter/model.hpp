// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_FILTER_MODEL_HPP
#define ALLOY_FILTER_MODEL_HPP

#include "../config.hpp"
#include "../detail/invocable.hpp"

namespace alloy {
    template<typename S>
    struct filter : detail::invocable<S> {
        using base = detail::invocable<S>;

        using base::base;
        using base::operator();
    };

    template<typename S>
    filter(S &&)->filter<S>;
}

#endif