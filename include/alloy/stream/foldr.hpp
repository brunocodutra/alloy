#ifndef ALLOY_STREAM_FOLDR_HPP
#define ALLOY_STREAM_FOLDR_HPP

#include "../config.hpp"
#include "../detail.hpp"
#include "../stream/model.hpp"

namespace alloy {
    inline constexpr auto foldr = [](auto&& f, auto&& init) {
        stream impl = [&f, &init](auto&& snk) noexcept {
            return [&f, &init, &snk](auto&&... args) -> decltype(auto) {
                return detail::invoke(static_cast<decltype(snk)>(snk),
                    detail::foldr(static_cast<decltype(f)>(f),
                        static_cast<decltype(args)>(args)...,
                        static_cast<decltype(init)>(init)));
            };
        };

        return impl;
    };
}

#endif
