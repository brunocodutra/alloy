// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_SOURCE_UNPACK_HPP
#define ALLOY_SOURCE_UNPACK_HPP

#include "../config.hpp"
#include "../defer.hpp"
#include "../detail/traits.hpp"
#include "../source/join.hpp"
#include "../source/model.hpp"

#include<tuple>
#include<variant>

namespace alloy::detail {
    template<typename Variant,
        requires<instanceof<Variant, std::variant>> = valid
    >
    constexpr auto unpack(Variant&& variant) noexcept {
        return [&variant](auto&& snk) -> decltype(auto) {
            return std::visit(
                static_cast<decltype(snk)>(snk),
                static_cast<Variant&&>(variant)
            );
        };
    }

    template<typename Tuple,
        requires<instanceof<Tuple, std::tuple>> = valid
    >
    constexpr auto unpack(Tuple&& tuple) noexcept {
        return [&tuple](auto&& snk) -> decltype(auto) {
            return std::apply(
                static_cast<decltype(snk)>(snk),
                static_cast<Tuple&&>(tuple)
            );
        };
    }
}

namespace alloy {
    inline constexpr auto unpack = [](auto&&... xs) {
        return source{defer(join, detail::unpack(static_cast<decltype(xs)>(xs))...)};
    };
}

#endif
