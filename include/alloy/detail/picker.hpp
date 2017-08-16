// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_DETAIL_PICKER_HPP
#define ALLOY_DETAIL_PICKER_HPP

#include "../config.hpp"
#include "../detail/args.hpp"
#include "../detail/invoke.hpp"

namespace alloy::detail {
    template<typename... Is>
    struct picker {
        template<typename R, typename F, typename... Args>
        static constexpr R dispatch(F&& f, Args&&... args) {
            return invoke(static_cast<F&&>(f),
                args_t<Args&&...>{static_cast<Args&&>(args)...}[Is{}]...);
        }

        template<typename F, typename... Args>
        static constexpr decltype(auto) dispatch(F&& f, Args&&... args) {
            return invoke(static_cast<F&&>(f),
                args_t<Args&&...>{static_cast<Args&&>(args)...}[Is{}]...);
        }
    };
}

#endif
