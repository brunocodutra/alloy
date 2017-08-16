// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_FILTER_AT_HPP
#define ALLOY_FILTER_AT_HPP

#include "../config.hpp"
#include "../constant.hpp"
#include "../detail.hpp"
#include "../filter/model.hpp"

namespace alloy::detail {
    template<typename... Is>
    constexpr auto at(Is&&... is) noexcept {
        return [&is...](auto&& snk) noexcept {
            return [&is..., &snk](auto&&... args) -> decltype(auto) {
                using namespace metal;

                using N = number<sizeof...(is)>;
                using Args = list<decltype(args)...>;

                using R = cascade<combine<Args, N>, lambda<std::common_type_t>,
                    partial<lambda<invoke_t>, decltype(snk)>>;

                using Dispatcher = cascade<combine<indices<Args>, N>,
                    lambda<dispatcher>, lambda<picker>>;

                return Dispatcher::template dispatch<R>(
                    foldl(
                        [](std::size_t i, std::size_t j) {
                            return sizeof...(args) * i + j;
                        },
                        0U, static_cast<Is&&>(is)...),
                    static_cast<decltype(snk)>(snk),
                    static_cast<decltype(args)>(args)...);
            };
        };
    }

    template<auto... is>
    constexpr auto at(constant<is>...) noexcept {
        return [](auto&& snk) noexcept {
            return [&snk](auto&&... args) -> decltype(auto) {
                return picker<constant<is>...>::template dispatch(
                    static_cast<decltype(snk)>(snk),
                    static_cast<decltype(args)>(args)...);
            };
        };
    }

    constexpr auto at() noexcept {
        return [](auto&& snk) noexcept {
            return [&snk](auto&&... args) -> decltype(auto) {
                return picker<>::template dispatch(
                    static_cast<decltype(snk)>(snk),
                    static_cast<decltype(args)>(args)...);
            };
        };
    }
}

namespace alloy {
    inline constexpr auto at = [](auto&&... is) {
        return filter{detail::at(static_cast<decltype(is)>(is)...)};
    };
}

#endif
