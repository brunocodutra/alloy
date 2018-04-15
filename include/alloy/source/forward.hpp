#ifndef ALLOY_SOURCE_FORWARD_HPP
#define ALLOY_SOURCE_FORWARD_HPP

#include "../config.hpp"
#include "../detail.hpp"
#include "../source/model.hpp"

namespace alloy::detail {
    template<typename... Vs>
    constexpr decltype(auto) forward(Vs&&... vs) noexcept {
        return source{detail::args_t<Vs&&...>{static_cast<Vs&&>(vs)...}};
    }
}

namespace alloy {
    inline constexpr auto forward = [](auto&&... xs) noexcept {
        return detail::forward(static_cast<decltype(xs)>(xs)...);
    };
}

#endif
