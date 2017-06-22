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
    struct picker {
        template<typename R, typename F, typename... Args>
        static constexpr R dispatch(F&& snk, Args&&... args) {
            return invoke(
                static_cast<F&&>(snk),
                forward(static_cast<Args&&>(args)...)[Is{}]...
            );
        }
    };

    template<typename... Is>
    constexpr auto at(Is&&... is) noexcept {
        return [&is...](auto&& snk) noexcept {
            return [&is..., &snk](auto&&... args) -> decltype(auto) {
                using R = metal::cascade<
                    metal::combine<
                        metal::list<decltype(args)...>,
                        metal::number<sizeof...(is)>
                    >,
                    metal::lambda<std::common_type_t>,
                    metal::partial<metal::lambda<invoke_t>, decltype(snk)>
                >;

                using Dispatcher = metal::cascade<
                    metal::combine<
                        metal::indices<metal::list<decltype(args)...>>,
                        metal::number<sizeof...(is)>
                    >,
                    metal::lambda<dispatcher>,
                    metal::lambda<picker>
                >;

                return Dispatcher::template dispatch<R>(
                    foldl([](std::size_t i, std::size_t j) {
                        return sizeof...(args)*i + j;
                    }, static_cast<Is&&>(is)...),
                    static_cast<decltype(snk)>(snk),
                    static_cast<decltype(args)>(args)...
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

                return picker<constant<is>...>::template dispatch<R>(
                    static_cast<decltype(snk)>(snk),
                    static_cast<decltype(args)>(args)...
                );
            };
        };
    }

    constexpr auto at() noexcept {
        return [](auto&& snk) noexcept {
            return [&snk](auto&&...) -> decltype(auto) {
                return invoke(static_cast<decltype(snk)>(snk));
            };
        };
    }
}

namespace alloy {
    inline constexpr auto at = [](auto&&... is) {
        return stream{detail::at(static_cast<decltype(is)>(is)...)};
    };
}

#endif
