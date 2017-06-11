// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_MODEL_HPP
#define ALLOY_MODEL_HPP

#include "config.hpp"
#include "detail.hpp"
#include "external.hpp"

namespace alloy::detail {
    template<typename F, typename = valid_t>
    struct invocable {
        F f;

        constexpr invocable() = delete;
        constexpr invocable(invocable&&) = default;
        constexpr invocable(invocable const&) = default;

        constexpr invocable(F&& f)
            : f{static_cast<F&&>(f)}
        {}

        template<typename... Args>
        constexpr decltype(auto) operator ()(Args&&... args) & {
            return detail::invoke(static_cast<F&>(f), static_cast<Args&&>(args)...);
        }

        template<typename... Args>
        constexpr decltype(auto) operator ()(Args&&... args) const& {
            return detail::invoke(static_cast<F const&>(f), static_cast<Args&&>(args)...);
        }

        template<typename... Args>
        constexpr decltype(auto) operator ()(Args&&... args) && {
            return detail::invoke(static_cast<F&&>(f), static_cast<Args&&>(args)...);
        }

        template<typename... Args>
        constexpr decltype(auto) operator ()(Args&&... args) const&& {
            return detail::invoke(static_cast<F const&&>(f), static_cast<Args&&>(args)...);
        }
    };

    template<typename F>
    struct invocable<F, requires<inheritable<F>>> : F {
        using base = F;
        using base::base;

        constexpr invocable(F&& f)
            : base{static_cast<F&&>(f)}
        {}
    };

    template<typename S>
    struct source : invocable<S> {
        using base = invocable<S>;

        using base::base;
        using base::operator ();
    };

    template<typename S>
    source(S&&) -> source<S>;

    template<typename S>
    struct sink : invocable<S> {
        using base = invocable<S>;

        using base::base;
        using base::operator ();
    };

    template<typename S>
    sink(S&&) -> sink<S>;

    template<typename S>
    struct stream : invocable<S> {
        using base = invocable<S>;

        using base::base;
        using base::operator ();
    };

    template<typename S>
    stream(S&&) -> stream<S>;

    template<typename S, typename T,
        requires<instanceof<S, stream>> = valid,
        requires<instanceof<T, stream>> = valid
    >
    constexpr decltype(auto) operator <<(S&& s, T&& t) {
        return stream{
            [&s, &t](auto&& snk) -> decltype(auto) {
                return static_cast<decltype(snk)>(snk)
                    << static_cast<S&&>(s)
                    << static_cast<T&&>(t);
            }
        };
    }

    template<typename Stm, typename Src,
        requires<instanceof<Stm, stream>> = valid,
        requires<!instanceof<Src, stream>> = valid
    >
    constexpr decltype(auto) operator <<(Stm&& stm, Src&& src) {
        return source{
            [&stm, &src](auto&& snk) -> decltype(auto) {
                return static_cast<decltype(snk)>(snk)
                    << static_cast<Stm&&>(stm)
                    << static_cast<Src&&>(src);
            }
        };
    }

    template<typename Snk, typename Stm,
        requires<!instanceof<Snk, stream>> = valid,
        requires<instanceof<Stm, stream>> = valid
    >
    constexpr decltype(auto) operator <<(Snk&& snk, Stm&& stm) {
        return sink{detail::invoke(static_cast<Stm&&>(stm), static_cast<Snk&&>(snk))};
    }

    template<typename Snk, typename Src,
        requires<!instanceof<Snk, stream>> = valid,
        requires<!instanceof<Src, stream>> = valid,
        requires<instanceof<Snk, sink> || instanceof<Src, source>> = valid
    >
    constexpr decltype(auto) operator <<(Snk&& snk, Src&& src) {
        return detail::invoke(static_cast<Src&&>(src), static_cast<Snk&&>(snk));
    }
}

namespace alloy {
    using detail::source;
    using detail::sink;
    using detail::stream;
}

#endif
