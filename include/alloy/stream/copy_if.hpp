// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_STREAM_COPY_IF_HPP
#define ALLOY_STREAM_COPY_IF_HPP

#include "../config.hpp"
#include "../detail/dispatcher.hpp"
#include "../detail/invoke.hpp"
#include "../detail/picker.hpp"
#include "../detail/traits.hpp"
#include "../stream/model.hpp"

namespace alloy::detail {
    template<typename F>
    constexpr auto copy_if(F&& f) noexcept {
        return [&f](auto&& snk) noexcept {
            return [&f, &snk](auto&&... args) -> decltype(auto) {
                using Args = metal::list<decltype(args)...>;

                using R = metal::cascade<metal::powerset<Args>,
                    metal::lambda<std::common_type_t>,
                    metal::partial<metal::lambda<invoke_t>, decltype(snk)>>;

                using Dispatcher =
                    metal::cascade<metal::powerset<metal::indices<Args>>,
                        metal::lambda<dispatcher>, metal::lambda<picker>>;

                return Dispatcher::template dispatch<R>(
                    foldr(
                        [&f](auto&& arg, std::size_t j) {
                            bool k = invoke(static_cast<decltype(f)>(f),
                                static_cast<decltype(arg)>(arg));

                            return 2 * j + k;
                        },
                        static_cast<decltype(args)&&>(args)..., 0U),
                    static_cast<decltype(snk)>(snk),
                    static_cast<decltype(args)>(args)...);
            };
        };
    }
}

namespace alloy {
    inline constexpr auto copy_if = [](auto&& f) {
        return stream{detail::copy_if(static_cast<decltype(f)>(f))};
    };
}

#endif
