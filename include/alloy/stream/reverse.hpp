#ifndef ALLOY_STREAM_REVERSE_HPP
#define ALLOY_STREAM_REVERSE_HPP

#include "../config.hpp"
#include "../detail.hpp"
#include "../stream/model.hpp"

namespace alloy::detail {
    constexpr auto reverse() noexcept {
        return [](auto&& snk) noexcept {
            return [&snk](auto&&... args) -> decltype(auto) {
                using namespace metal;

                using Picker = apply<lambda<picker>,
                    metal::reverse<indices<list<decltype(args)...>>>>;

                return Picker::template dispatch(
                    static_cast<decltype(snk)>(snk),
                    static_cast<decltype(args)>(args)...);
            };
        };
    }
}

namespace alloy {
    inline constexpr auto reverse = []() { return stream{detail::reverse()}; };
}

#endif
