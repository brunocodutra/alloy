// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_DETAIL_FUNCTIONAL_HPP
#define ALLOY_DETAIL_FUNCTIONAL_HPP

#include "../config.hpp"
#include "../detail/traits.hpp"

#define ALLOY_RETURN(...) \
    -> decltype(__VA_ARGS__) { \
        return __VA_ARGS__; \
    } \
/**/

namespace alloy::detail {
    template<typename Base, typename F, typename Ref, typename... Args,
        requires<std::is_function<F>::value> = valid,
        requires<std::is_base_of<Base, strip<Ref>>::value> = valid
    >
    constexpr auto invoke(F Base::* pmf, Ref&& ref, Args&&... args) ALLOY_RETURN(
        (static_cast<Ref&&>(ref).*pmf)(static_cast<Args&&>(args)...)
    )

    template<typename Base, typename F, typename Ptr, typename... Args,
        requires<std::is_function<F>::value> = valid,
        requires<!std::is_base_of<Base, strip<Ptr>>::value> = valid
    >
    constexpr auto invoke(F Base::* pmf, Ptr&& ptr, Args&&... args) ALLOY_RETURN(
        ((*static_cast<Ptr&&>(ptr)).*pmf)(static_cast<Args&&>(args)...)
    )

    template<typename Base, typename D, typename Ref,
        requires<!std::is_function<D>::value> = valid,
        requires<std::is_base_of<Base, strip<Ref>>::value> = valid
    >
    constexpr auto invoke(D Base::* pmd, Ref&& ref) noexcept ALLOY_RETURN(
        static_cast<Ref&&>(ref).*pmd
    )

    template<typename Base, typename D, typename Ptr,
        requires<!std::is_function<D>::value> = valid,
        requires<!std::is_base_of<Base, strip<Ptr>>::value> = valid
    >
    constexpr auto invoke(D Base::* pmd, Ptr&& ptr) noexcept ALLOY_RETURN(
        (*static_cast<Ptr&&>(ptr)).*pmd
    )

    template<typename F, typename... Args>
    constexpr auto invoke(F&& f, Args&&... args) ALLOY_RETURN(
        static_cast<F&&>(f)(static_cast<Args&&>(args)...)
    )

    template<typename F, typename... Args>
    using invoke_t = decltype(detail::invoke(std::declval<F>(), std::declval<Args>()...));

    template<typename F, typename = valid_t>
    struct invocable {
        F f;

        constexpr invocable() = delete;
        constexpr invocable(invocable&&) = default;
        constexpr invocable(invocable const&) = default;

        constexpr invocable(F&& f)
            : f{static_cast<F&&>(f)}
        {}

        template<typename... Args>
        constexpr decltype(auto) operator ()(Args&&... args) & {
            return detail::invoke(static_cast<F&>(f), static_cast<Args&&>(args)...);
        }

        template<typename... Args>
        constexpr decltype(auto) operator ()(Args&&... args) const& {
            return detail::invoke(static_cast<F const&>(f), static_cast<Args&&>(args)...);
        }

        template<typename... Args>
        constexpr decltype(auto) operator ()(Args&&... args) && {
            return detail::invoke(static_cast<F&&>(f), static_cast<Args&&>(args)...);
        }

        template<typename... Args>
        constexpr decltype(auto) operator ()(Args&&... args) const&& {
            return detail::invoke(static_cast<F const&&>(f), static_cast<Args&&>(args)...);
        }
    };

    template<typename F>
    struct invocable<F, requires<inheritable<F>>> : F {
        using base = F;
        using base::base;

        constexpr invocable(F&& f)
            : base{static_cast<F&&>(f)}
        {}
    };

    struct nil {};

    template<typename F, typename Arg>
    struct folder {
        F&& f;
        Arg&& arg;

        constexpr F&& operator +() noexcept {
            return static_cast<F&&>(f);
        }

        constexpr Arg&& operator *() noexcept {
            return static_cast<Arg&&>(arg);
        }

        friend constexpr decltype(auto) operator >>=(nil, folder ref) noexcept {
            return *ref;
        }

        template<typename State>
        friend constexpr decltype(auto) operator >>=(State&& state, folder ref) {
            return detail::invoke(+ref, static_cast<State&&>(state), *ref);
        }

        friend constexpr decltype(auto) operator <<=(folder ref, nil) noexcept {
            return *ref;
        }

        template<typename State>
        friend constexpr decltype(auto) operator <<=(folder ref, State&& state) {
            return detail::invoke(+ref, *ref, static_cast<State&&>(state));
        }
    };

    template<typename F, typename... Args>
    constexpr decltype(auto) foldl(F&& f, Args&&... args) {
        return (nil{} >>= ... >>= folder<F, Args>{
            static_cast<F&&>(f),
            static_cast<Args&&>(args)
        });
    }

    template<typename F, typename... Args>
    constexpr decltype(auto) foldr(F&& f, Args&&... args) {
        return (folder<F, Args>{
            static_cast<F&&>(f),
            static_cast<Args&&>(args)
        } <<= ... <<= nil{});
    }
}

#undef ALLOY_RETURN
#endif
