// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_FILTER_COPY_IF_HPP
#define ALLOY_FILTER_COPY_IF_HPP

#include "../config.hpp"
#include "../constant.hpp"
#include "../detail/dispatcher.hpp"
#include "../detail/invoke.hpp"
#include "../detail/picker.hpp"
#include "../detail/traits.hpp"
#include "../filter/model.hpp"

namespace alloy::detail {
    template<typename... Ns>
    constexpr auto copy_if_impl(Ns&&... ns) noexcept {
        return [&ns...](auto&& snk) noexcept {
            return [&ns..., &snk](auto&&... args) -> decltype(auto) {
                using Args = metal::list<decltype(args)...>;

                using R = metal::cascade<metal::powerset<Args>,
                    metal::lambda<std::common_type_t>,
                    metal::partial<metal::lambda<invoke_t>, decltype(snk)>>;

                using Dispatcher =
                    metal::cascade<metal::powerset<metal::indices<Args>>,
                        metal::lambda<dispatcher>, metal::lambda<picker>>;

                return Dispatcher::template dispatch<R>(
                    foldr([](bool j, std::size_t i) { return 2 * i + j; },
                        !!static_cast<Ns&&>(ns)..., 0U),
                    static_cast<decltype(snk)>(snk),
                    static_cast<decltype(args)>(args)...);
            };
        };
    }

    template<auto... ns>
    constexpr auto copy_if_impl(constant<ns>...) noexcept {
        using Is = metal::copy_if<metal::indices<metal::numbers<ns...>>,
            metal::partial<metal::lambda<metal::at>, metal::numbers<ns...>>>;

        return [](auto&& snk) noexcept {
            return [&snk](auto&&... args) -> decltype(auto) {
                using Picker = metal::apply<metal::lambda<picker>, Is>;
                return Picker::template dispatch(
                    static_cast<decltype(snk)>(snk),
                    static_cast<decltype(args)>(args)...);
            };
        };
    }

    constexpr auto copy_if_impl() noexcept {
        return [](auto&& snk) noexcept {
            return [&snk](auto&&... args) -> decltype(auto) {
                return picker<>::template dispatch(
                    static_cast<decltype(snk)>(snk),
                    static_cast<decltype(args)>(args)...);
            };
        };
    }
}

/* clang-format off */
namespace alloy {
    inline constexpr auto copy_if = [](auto&& f) {
        return filter{
            [&f](auto&& snk) noexcept {
                return [&f, &snk](auto&&... args) -> decltype(auto) {
                    return detail::copy_if_impl(
                        detail::invoke(static_cast<decltype(f)>(f),
                            static_cast<decltype(args)>(args))...)(
                                static_cast<decltype(snk)>(snk))(
                                    static_cast<decltype(args)>(args)...);
                };
            }
        };
    };
}
/* clang-format off */

#endif
