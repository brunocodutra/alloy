#ifndef ALLOY_DETAIL_INVOKE_HPP
#define ALLOY_DETAIL_INVOKE_HPP

#include "../config.hpp"
#include "../detail/traits.hpp"

#define ALLOY_RETURN(...) \
    ->decltype(__VA_ARGS__) { \
        return __VA_ARGS__; \
    }

namespace alloy::detail {
    /* clang-format off */

    template<typename Base, typename F, typename Ref, typename... Args,
        requires<std::is_function<F>::value> = valid,
        requires<std::is_base_of<Base, strip<Ref>>::value> = valid
    >
    constexpr auto _invoke(F Base::* pmf, Ref&& ref, Args&&... args)
    ALLOY_RETURN((static_cast<Ref&&>(ref).*pmf)(static_cast<Args&&>(args)...))

    template<typename Base, typename F, typename Ptr, typename... Args,
        requires<std::is_function<F>::value> = valid,
        requires<!std::is_base_of<Base, strip<Ptr>>::value> = valid
    >
    constexpr auto _invoke(F Base::* pmf, Ptr&& ptr, Args&&... args)
    ALLOY_RETURN((*static_cast<Ptr&&>(ptr)).*pmf(static_cast<Args&&>(args)...))

    template<typename Base, typename D, typename Ref,
        requires<!std::is_function<D>::value> = valid,
        requires<std::is_base_of<Base, strip<Ref>>::value> = valid
    >
    constexpr auto _invoke(D Base::* pmd, Ref&& ref) noexcept
    ALLOY_RETURN(static_cast<Ref&&>(ref).*pmd)

    template<typename Base, typename D, typename Ptr,
        requires<!std::is_function<D>::value> = valid,
        requires<!std::is_base_of<Base, strip<Ptr>>::value> = valid
    >
    constexpr auto _invoke(D Base::* pmd, Ptr&& ptr) noexcept
    ALLOY_RETURN((*static_cast<Ptr&&>(ptr)).*pmd)

    template<typename F, typename... Args>
    constexpr auto _invoke(F&& f, Args&&... args)
    ALLOY_RETURN(static_cast<F&&>(f)(static_cast<Args&&>(args)...))

    template<typename F, typename... Args>
    using invoke_t =
        decltype(_invoke(std::declval<F>(), std::declval<Args>()...));

    /* clang-format on */

    inline constexpr auto invoke = [](auto&&... args) -> decltype(auto) {
        return detail::_invoke(static_cast<decltype(args)>(args)...);
    };
}

#undef ALLOY_RETURN
#endif
