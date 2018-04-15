#ifndef ALLOY_SOURCE_JOIN_HPP
#define ALLOY_SOURCE_JOIN_HPP

#include "../config.hpp"
#include "../detail.hpp"
#include "../source/forward.hpp"
#include "../source/model.hpp"

namespace alloy::detail {
    template<typename Src>
    constexpr auto buffer(Src&& src) noexcept {
        return [&src](auto&& snk) noexcept {
            return [&src, &snk](auto&&... xs) -> decltype(auto) {
                return invoke(
                    static_cast<Src&&>(src), [&snk, &xs...](auto&&... ys) {
                        return invoke(static_cast<decltype(snk)>(snk),
                            static_cast<decltype(xs)>(xs)...,
                            static_cast<decltype(ys)>(ys)...);
                    });
            };
        };
    }
}

namespace alloy {
    inline constexpr auto join = [](auto&&... srcs) {
        return source{[&srcs...](auto&& snk) -> decltype(auto) {
            return detail::foldr(detail::invoke, forward(),
                detail::buffer(static_cast<decltype(srcs)>(srcs))...,
                static_cast<decltype(snk)>(snk));
        }};
    };
}

#endif
