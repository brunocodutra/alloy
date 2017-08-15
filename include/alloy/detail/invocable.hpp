// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_DETAIL_INVOCABLE_HPP
#define ALLOY_DETAIL_INVOCABLE_HPP

#include "../config.hpp"
#include "../detail/traits.hpp"

namespace alloy::detail {
    template<typename F, typename = valid_t>
    struct invocable {
        F f;

        constexpr invocable() = delete;
        constexpr invocable(invocable&&) = default;
        constexpr invocable(invocable const&) = default;

        constexpr invocable(F&& f) : f{static_cast<F&&>(f)} {
        }

        template<typename... Args>
        constexpr decltype(auto) operator()(Args&&... args) & {
            return invoke(static_cast<F&>(f), static_cast<Args&&>(args)...);
        }

        template<typename... Args>
        constexpr decltype(auto) operator()(Args&&... args) const & {
            return invoke(
                static_cast<F const&>(f), static_cast<Args&&>(args)...);
        }

        template<typename... Args>
        constexpr decltype(auto) operator()(Args&&... args) && {
            return invoke(static_cast<F&&>(f), static_cast<Args&&>(args)...);
        }

        template<typename... Args>
        constexpr decltype(auto) operator()(Args&&... args) const && {
            return invoke(
                static_cast<F const&&>(f), static_cast<Args&&>(args)...);
        }
    };

    template<typename F>
    struct invocable<F, requires<inheritable<F>>> : F {
        using base = F;
        using base::base;

        constexpr invocable(F&& f) : base{static_cast<F&&>(f)} {
        }
    };
}

#endif
