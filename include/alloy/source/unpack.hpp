// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_SOURCE_UNPACK_HPP
#define ALLOY_SOURCE_UNPACK_HPP

#include "../config.hpp"
#include "../detail/dispatcher.hpp"
#include "../detail/invoke.hpp"
#include "../detail/traits.hpp"
#include "../source/forward.hpp"
#include "../source/model.hpp"

#include <tuple>
#include <variant>

namespace alloy::detail {
    template<typename, typename>
    struct unpacker {};

    template<typename... Is, typename... Js>
    struct unpacker<metal::list<Is...>, metal::list<Js...>> {
        template<typename R, typename F, typename... Args>
        static constexpr R dispatch(F&& snk, Args&&... args) {
            return invoke(static_cast<F&&>(snk),
                std::get<Js::value>(
                    forward(static_cast<Args&&>(args)...)[Is{}])...);
        }
    };

    template<typename, typename, typename>
    struct _inner_impl {};

    template<typename T, typename... Ts, typename U>
    struct _inner_impl<T, std::tuple<Ts...>, U> {
        using type =
            metal::transpose<metal::list<metal::invoke<T, transfer<U, Ts>...>>>;
    };

    template<typename T, typename... Ts, typename U>
    struct _inner_impl<T, std::variant<Ts...>, U> {
        using type = metal::list<metal::invoke<T, transfer<U, Ts>...>>;
    };

    template<typename T, typename... Xs>
    using inner = metal::apply<metal::lambda<metal::cartesian>,
        metal::join<metal::eval<_inner_impl<T, strip<Xs>, Xs&&>>...>>;

    template<typename, typename>
    struct _outer_impl {};

    template<typename N, typename... Ts>
    struct _outer_impl<N, std::tuple<Ts...>> {
        using type = metal::repeat<N, metal::size<metal::list<Ts...>>>;
    };

    template<typename N, typename... Ts>
    struct _outer_impl<N, std::variant<Ts...>> {
        using type = metal::list<N>;
    };

    template<typename T, typename... Xs>
    using outer = metal::list<metal::apply<metal::lambda<metal::join>,
        metal::transform<metal::lazy<_outer_impl>,
            metal::invoke<T, Xs...>,
            metal::list<strip<Xs>...>>>>;

    template<typename... Xs>
    constexpr auto unpack(Xs&&... xs) noexcept {
        return [&xs...](auto&& snk) -> decltype(auto) {
            using R = metal::cascade<inner<metal::lambda<metal::list>, Xs...>,
                metal::lambda<std::common_type_t>,
                metal::partial<metal::lambda<invoke_t>, decltype(snk)>>;

            using indices = metal::bind<metal::lambda<metal::indices>,
                metal::lambda<metal::list>>;

            using Dispatcher = metal::cascade<
                metal::cartesian<outer<indices, Xs...>, inner<indices, Xs...>>,
                metal::lambda<dispatcher>, metal::lambda<unpacker>>;

            constexpr std::size_t N = (... + instanceof <Xs, std::variant>);

            return Dispatcher::template dispatch<R>(
                foldl(
                    [](std::size_t i, auto&& x) {
                        if constexpr(instanceof <decltype(x), std::variant>)
                            return N * i + static_cast<decltype(x)>(x).index();
                        else
                            return i;
                    },
                    0U, static_cast<Xs&&>(xs)...),
                static_cast<decltype(snk)>(snk), static_cast<Xs&&>(xs)...);
        };
    }

    constexpr auto unpack() noexcept {
        return [](auto&& snk) noexcept {
            return invoke(static_cast<decltype(snk)>(snk));
        };
    }
}

namespace alloy {
    inline constexpr auto unpack = [](auto&&... xs) {
        return source{detail::unpack(static_cast<decltype(xs)>(xs)...)};
    };
}

#endif
