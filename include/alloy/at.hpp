// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_AT_HPP
#define ALLOY_AT_HPP

#include "config.hpp"
#include "constant.hpp"
#include "detail.hpp"
#include "forward.hpp"
#include "model.hpp"

namespace alloy::detail {
    template<typename... Is>
    struct dispatcher {
        template<typename R, typename F, typename... Args>
        static constexpr R dispatch(F&& snk, Args&&... args) {
            return detail::invoke(
                static_cast<F&&>(snk),
                forward(static_cast<Args&&>(args)...)[Is{}]...
            );
        }
    };

    template<typename... Dispatchers>
    struct dispatchers {
        template<typename R, typename F, typename... Args>
        static constexpr R dispatch(std::size_t i, F&& f, Args&&... args) {
            using Sig = R(F&&, Args&&...);
            constexpr Sig* dispatchers[] = {
                &Dispatchers::template dispatch<R, F, Args...>...
            };

            return dispatchers[i](
                static_cast<F&&>(f),
                static_cast<Args&&>(args)...
            );
        }
    };

    template<typename... Is>
    constexpr auto at(Is&&... is) noexcept {
        return [&is...](auto&& snk) noexcept {
            return [&is..., &snk](auto&&... args) -> decltype(auto) {
                using N = metal::number<sizeof...(is)>;

                using R = combine<
                    metal::lambda<std::common_type_t>,
                    metal::partial<metal::lambda<invoke_t>, decltype(snk)>,
                    repeat<N, metal::list<decltype(args)...>>
                >;

                using Dispatchers = combine<
                    metal::lambda<dispatchers>,
                    metal::lambda<dispatcher>,
                    repeat<N, metal::indices<metal::list<decltype(args)...>>>
                >;

                return Dispatchers::template dispatch<R>(
                    foldr([](std::size_t i, std::size_t j) {
                        return i + sizeof...(args)*j;
                    }, static_cast<Is&&>(is)...),
                    static_cast<decltype(snk)&&>(snk),
                    static_cast<decltype(args)&&>(args)...
                );
            };
        };
    }

    template<auto... is>
    constexpr auto at(constant<is>...) noexcept {
        return [](auto&& snk) noexcept {
            return [&snk](auto&&... args) -> decltype(auto) {
                using R = invoke_t<
                    decltype(snk),
                    metal::at<
                        metal::list<decltype(args)...>,
                        metal::number<is>
                    >...
                >;

                return dispatcher<constant<is>...>::template dispatch<R>(
                    static_cast<decltype(snk)&&>(snk),
                    static_cast<decltype(args)&&>(args)...
                );
            };
        };
    }

    constexpr auto at() noexcept {
        return [](auto&& snk) noexcept {
            return [&snk](auto&&...) -> decltype(auto) {
                return detail::invoke(static_cast<decltype(snk)&&>(snk));
            };
        };
    }
}

namespace alloy {
    inline constexpr auto at = [](auto&&... is) {
        return stream{detail::at(static_cast<decltype(is)&&>(is)...)};
    };
}

#endif
