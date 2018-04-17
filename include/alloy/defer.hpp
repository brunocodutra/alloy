#ifndef ALLOY_DEFER_HPP
#define ALLOY_DEFER_HPP

#include "config.hpp"
#include "detail.hpp"

namespace alloy::detail {
    template<typename F, typename... Xs>
    struct deferred : args_t<F, Xs...> {
        using base = args_t<F, Xs...>;

        using base::base;

        template<typename... Args>
        constexpr decltype(auto) operator()(Args&&... args) & {
            return invoke(static_cast<base&>(*this)(invoke),
                static_cast<Args&&>(args)...);
        }

        template<typename... Args>
        constexpr decltype(auto) operator()(Args&&... args) const & {
            return invoke(static_cast<base const&>(*this)(invoke),
                static_cast<Args&&>(args)...);
        }

        template<typename... Args>
        constexpr decltype(auto) operator()(Args&&... args) && {
            return invoke(static_cast<base&&>(*this)(invoke),
                static_cast<Args&&>(args)...);
        }

        template<typename... Args>
        constexpr decltype(auto) operator()(Args&&... args) const && {
            return invoke(static_cast<base const&&>(*this)(invoke),
                static_cast<Args&&>(args)...);
        }
    };

    template<typename F, typename... Xs>
    deferred(F&&, Xs&&...)->deferred<F, Xs...>;
}

namespace alloy {
    inline constexpr auto defer = [](auto&& f, auto&&... xs) {
        return detail::deferred{
            static_cast<decltype(f)>(f), static_cast<decltype(xs)>(xs)...};
    };
}

#endif
