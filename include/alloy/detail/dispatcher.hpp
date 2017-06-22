// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_DETAIL_DISPATCHER_HPP
#define ALLOY_DETAIL_DISPATCHER_HPP

#include "../config.hpp"

#include <cstddef>

namespace alloy::detail {
    template<typename... Targets>
    struct dispatcher {
        template<typename R, typename... Args>
        static constexpr R dispatch(std::size_t i, Args&&... args) {
            constexpr R(*targets[])(Args&&...) = {
                &Targets::template dispatch<R, Args...>...
            };

            return targets[i](static_cast<Args&&>(args)...);
        }
    };
}

#endif
