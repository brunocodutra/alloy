#ifndef ALLOY_STREAM_TRANSFORM_HPP
#define ALLOY_STREAM_TRANSFORM_HPP

#include "../config.hpp"
#include "../detail.hpp"
#include "../stream/model.hpp"

namespace alloy::detail {
    template<typename F>
    constexpr auto transform(F&& f) noexcept {
        return [&f](auto&& snk) noexcept {
            return [&f, &snk](auto&&... args) -> decltype(auto) {
                return invoke(static_cast<decltype(snk)>(snk),
                    invoke(static_cast<decltype(f)>(f),
                        static_cast<decltype(args)>(args))...);
            };
        };
    }
}

namespace alloy {
    inline constexpr auto transform = [](auto&& f) {
        return stream{detail::transform(static_cast<decltype(f)>(f))};
    };
}

#endif
