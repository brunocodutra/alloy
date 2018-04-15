#ifndef ALLOY_FILTER_COPY_IF_HPP
#define ALLOY_FILTER_COPY_IF_HPP

#include "../config.hpp"
#include "../constant.hpp"
#include "../detail.hpp"
#include "../filter/model.hpp"

namespace alloy::detail {
    template<typename... Ns>
    constexpr auto copy_if_impl(Ns&&... ns) noexcept {
        return [&ns...](auto&& snk) noexcept {
            return [&ns..., &snk](auto&&... args) -> decltype(auto) {
                using namespace metal;

                using Args = list<decltype(args)...>;

                using R = cascade<powerset<Args>, lambda<std::common_type_t>,
                    partial<lambda<invoke_t>, decltype(snk)>>;

                using Dispatcher = cascade<powerset<indices<Args>>,
                    lambda<dispatcher>, lambda<picker>>;

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
        using namespace metal;

        using Is = copy_if<indices<numbers<ns...>>,
            partial<lambda<metal::at>, numbers<ns...>>>;

        return [](auto&& snk) noexcept {
            return [&snk](auto&&... args) -> decltype(auto) {
                return apply<lambda<picker>, Is>::template dispatch(
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
