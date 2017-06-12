// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_DETAIL_ARGS_HPP
#define ALLOY_DETAIL_ARGS_HPP

#include "../config.hpp"
#include "../constant.hpp"
#include "../external.hpp"

#include "../detail/invoke.hpp"
#include "../detail/traits.hpp"

namespace alloy::detail {
    template<typename K, typename V = K, typename = valid_t>
    struct arg {
        V v;

        using type = V;

        constexpr arg() = delete;
        constexpr arg(arg&&) = default;
        constexpr arg(arg const&) = default;

        constexpr arg(V&& v)
            : v{static_cast<V&&>(v)}
        {}

        constexpr operator transfer<arg&, V>() & noexcept {
            return static_cast<transfer<arg&, V>>(v);
        }

        constexpr operator transfer<arg const&, V>() const& noexcept {
            return static_cast<transfer<arg const&, V>>(v);
        }

        constexpr operator transfer<arg&&, V>() && noexcept {
            return static_cast<transfer<arg&&, V>>(v);
        }

        constexpr operator transfer<arg const&&, V>() const&& noexcept {
            return static_cast<transfer<arg const&&, V>>(v);
        }
    };

    template<typename K, typename V>
    struct arg<K, V, requires<inheritable<V>>> : V {
        using type = V;

        constexpr arg() = delete;
        constexpr arg(arg&&) = default;
        constexpr arg(arg const&) = default;

        constexpr arg(V&& v)
            : V{static_cast<V&&>(v)}
        {}
    };

    template<typename...>
    struct args;

    template<typename... Ks, typename... Vs>
    struct args<arg<Ks, Vs>...> : arg<Ks, Vs>... {
        constexpr args(args&&) = default;
        constexpr args(args const&) = default;

        constexpr args(Vs&&... vs)
            : arg<Ks, Vs>{static_cast<Vs&&>(vs)}...
        {}

        template<auto i,
            typename Arg = metal::at<metal::as_list<args>, metal::number<i>>
        >
        constexpr decltype(auto) operator [](constant<i>) & noexcept {
            return args::at<Arg>(static_cast<args&>(*this));
        }

        template<auto i,
            typename Arg = metal::at<metal::as_list<args>, metal::number<i>>
        >
        constexpr decltype(auto) operator [](constant<i>) const& noexcept {
            return args::at<Arg>(static_cast<args const&>(*this));
        }

        template<auto i,
            typename Arg = metal::at<metal::as_list<args>, metal::number<i>>
        >
        constexpr decltype(auto) operator [](constant<i>) && noexcept {
            return args::at<Arg>(static_cast<args&&>(*this));
        }

        template<auto i,
            typename Arg = metal::at<metal::as_list<args>, metal::number<i>>
        >
        constexpr decltype(auto) operator [](constant<i>) const&& noexcept {
            return args::at<Arg>(static_cast<args const&&>(*this));
        }

        template<typename F>
        constexpr decltype(auto) operator ()(F&& f) & {
            return args::call(
                static_cast<args&>(*this),
                static_cast<F&&>(f)
            );
        }

        template<typename F>
        constexpr decltype(auto) operator ()(F&& f) const& {
            return args::call(
                static_cast<args const&>(*this),
                static_cast<F&&>(f)
            );
        }

        template<typename F>
        constexpr decltype(auto) operator ()(F&& f) && {
            return args::call(
                static_cast<args&&>(*this),
                static_cast<F&&>(f)
            );
        }

        template<typename F>
        constexpr decltype(auto) operator ()(F&& f) const&& {
            return args::call(
                static_cast<args const&&>(*this),
                static_cast<F&&>(f)
            );
        }

    private:
        template<typename Arg, typename Self>
        static constexpr decltype(auto) at(Self&& self) noexcept {
            return static_cast<transfer<Self&&, typename Arg::type>>(
                static_cast<transfer<Self&&, Arg>>(
                    static_cast<Self&&>(self)
                )
            );
        }

        template<typename Self, typename F>
        static constexpr decltype(auto) call(Self&& self, F&& f) {
            return detail::invoke(
                static_cast<F&&>(f),
                args::at<arg<Ks, Vs>>(static_cast<Self&&>(self))...
            );
        }
    };

    template<typename... Vs>
    using args_t = metal::apply<
        metal::lambda<args>,
        metal::transform<
            metal::lambda<arg>,
            metal::indices<metal::list<Vs...>>,
            metal::list<Vs...>
        >
    >;
}

#endif
