#ifndef ALLOY_DETAIL_FOLD_HPP
#define ALLOY_DETAIL_FOLD_HPP

#include "../config.hpp"
#include "../detail/invoke.hpp"

namespace alloy::detail {
    struct nil {};

    template<typename F, typename Arg>
    struct folder {
        F&& f;
        Arg&& arg;

        constexpr F&& operator+() noexcept {
            return static_cast<F&&>(f);
        }

        constexpr Arg&& operator*() noexcept {
            return static_cast<Arg&&>(arg);
        }

        friend constexpr decltype(auto) operator>>=(nil, folder ref) noexcept {
            return *ref;
        }

        template<typename State>
        friend constexpr decltype(auto) operator>>=(State&& state, folder ref) {
            return invoke(+ref, static_cast<State&&>(state), *ref);
        }

        friend constexpr decltype(auto) operator<<=(folder ref, nil) noexcept {
            return *ref;
        }

        template<typename State>
        friend constexpr decltype(auto) operator<<=(folder ref, State&& state) {
            return invoke(+ref, *ref, static_cast<State&&>(state));
        }
    };

    /* clang-format off */

    template<typename F, typename... Args>
    constexpr decltype(auto) foldl(F&& f, Args&&... args) {
        return (nil{} >>= ... >>= folder<F, Args>{
            static_cast<F&&>(f),
            static_cast<Args&&>(args)
        });
    }

    template<typename F, typename... Args>
    constexpr decltype(auto) foldr(F&& f, Args&&... args) {
        return (folder<F, Args>{
            static_cast<F&&>(f),
            static_cast<Args&&>(args)
        } <<= ... <<= nil{});
    }

    /* clang-format on */
}

#endif
