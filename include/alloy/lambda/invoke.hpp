// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_LAMBDA_INVOKE_HPP
#define ALLOY_LAMBDA_INVOKE_HPP

#include <alloy/config.hpp>
#include <alloy/number/number.hpp>

#include <alloy/detail/return.hpp>
#include <alloy/detail/traits.hpp>

namespace alloy::detail {
    template<typename Base, typename F, typename Ref, typename... Args,
        where<std::is_function<F>::value> = valid,
        where<std::is_base_of<Base, strip<Ref>>::value> = valid
    >
    constexpr auto invoke(F Base::* pmf, Ref&& ref, Args&&... args) ALLOY_RETURN(
        (static_cast<Ref&&>(ref).*pmf)(static_cast<Args&&>(args)...)
    )

    template <typename Base, typename F, typename Ptr, typename... Args,
        where<std::is_function<F>::value> = valid,
        where<!std::is_base_of<Base, strip<Ptr>>::value> = valid
    >
    constexpr auto invoke(F Base::* pmf, Ptr&& ptr, Args&&... args) ALLOY_RETURN(
        ((*static_cast<Ptr&&>(ptr)).*pmf)(static_cast<Args&&>(args)...)
    )

    template <typename Base, typename D, typename Ref,
        where<!std::is_function<D>::value> = valid,
        where<std::is_base_of<Base, strip<Ref>>::value> = valid
    >
    constexpr auto invoke(D Base::* pmd, Ref&& ref) ALLOY_RETURN(
        static_cast<Ref&&>(ref).*pmd
    )

    template <typename Base, typename D, typename Ptr,
        where<!std::is_function<D>::value> = valid,
        where<!std::is_base_of<Base, strip<Ptr>>::value> = valid
    >
    constexpr auto invoke(D Base::* pmd, Ptr&& ptr) ALLOY_RETURN(
        (*static_cast<Ptr&&>(ptr)).*pmd
    )

    template <typename Lbd, typename... Args>
    constexpr auto invoke(Lbd&& lbd, Args&&... args) ALLOY_RETURN(
        static_cast<Lbd&&>(lbd)(static_cast<Args&&>(args)...)
    )

    inline constexpr struct {
        template<typename... Args>
        constexpr auto operator ()(Args&&... args) ALLOY_RETURN(
            invoke(static_cast<Args&&>(args)...)
        )
    } _invoke{};

    template<typename Lbd, typename... Args>
    constexpr auto is_invocable(Lbd&& lbd, Args&&... args) noexcept
        -> decltype(
            void(invoke(static_cast<Lbd&&>(lbd), static_cast<Args&&>(args)...)),
            number<true>{}
        ) {
        return {};
    }

    template<typename... Args>
    constexpr number<false> is_invocable(Args&&...) noexcept {
        return {};
    }

    inline constexpr struct {
        template<typename... Args>
        constexpr auto operator ()(Args&&... args) const ALLOY_RETURN(
            is_invocable(static_cast<Args&&>(args)...)
        )
    } _is_invocable{};
}

namespace alloy {
    inline constexpr auto& invoke = detail::_invoke;
    inline constexpr auto& is_invocable = detail::_is_invocable;
}

#endif
