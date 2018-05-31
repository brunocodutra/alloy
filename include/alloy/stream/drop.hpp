#ifndef ALLOY_STREAM_DROP_HPP
#define ALLOY_STREAM_DROP_HPP

#include "../config.hpp"
#include "../constant.hpp"
#include "../detail.hpp"
#include "../stream/model.hpp"

namespace alloy::detail {
    template<typename I>
    constexpr auto drop(I&& i) noexcept {
        return [&i](auto&& snk) noexcept {
            return [&i, &snk](auto&&... args) -> decltype(auto) {
                using namespace metal;

                using Ns = iota<number<0>, number<sizeof...(args) + 1>>;
                using Args = list<decltype(args)...>;

                using R =
                    cascade<transform<partial<lambda<metal::drop>, Args>, Ns>,
                        lambda<common>,
                        partial<lambda<invoke_t>, decltype(snk)>>;

                using Dispatcher = cascade<
                    transform<partial<lambda<metal::drop>, indices<Args>>, Ns>,
                    lambda<dispatcher>, lambda<picker>>;

                return Dispatcher::template dispatch<R>(static_cast<I&&>(i),
                    static_cast<decltype(snk)>(snk),
                    static_cast<decltype(args)>(args)...);
            };
        };
    }

    template<auto i>
    constexpr auto drop(constant<i>) noexcept {
        return [](auto&& snk) noexcept {
            return [&snk](auto&&... args) -> decltype(auto) {
                using namespace metal;

                using Picker = apply<lambda<picker>,
                    metal::drop<indices<list<decltype(args)...>>, number<i>>>;

                return Picker::template dispatch(
                    static_cast<decltype(snk)>(snk),
                    static_cast<decltype(args)>(args)...);
            };
        };
    }
}

namespace alloy {
    inline constexpr auto drop = [](auto&& i) {
        return stream{detail::drop(static_cast<decltype(i)>(i))};
    };
}

#endif
