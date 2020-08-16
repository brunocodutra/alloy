#ifndef ALLOY_DETAIL_INVOKE_HPP
#define ALLOY_DETAIL_INVOKE_HPP

#include "../config.hpp"
#include "../detail/traits.hpp"

namespace alloy::detail {
template <typename Base, typename F, typename Ref, typename... Args,
    requires<std::is_function<F>::value> = valid,
    requires<std::is_base_of<Base, strip<Ref>>::value> = valid>
constexpr decltype(auto) _invoke(F Base::*pmf, Ref&& ref, Args&&... args)
{
    return (static_cast<Ref&&>(ref).*pmf)(static_cast<Args&&>(args)...);
}

template <typename Base, typename F, typename Ptr, typename... Args,
    requires<std::is_function<F>::value> = valid,
    requires<!std::is_base_of<Base, strip<Ptr>>::value> = valid>
constexpr decltype(auto) _invoke(F Base::*pmf, Ptr&& ptr, Args&&... args)
{
    return (*static_cast<Ptr&&>(ptr)).*pmf(static_cast<Args&&>(args)...);
}

template <typename Base, typename D, typename Ref,
    requires<!std::is_function<D>::value> = valid,
    requires<std::is_base_of<Base, strip<Ref>>::value> = valid>
constexpr decltype(auto) _invoke(D Base::*pmd, Ref&& ref) noexcept
{
    return static_cast<Ref&&>(ref).*pmd;
}

template <typename Base, typename D, typename Ptr,
    requires<!std::is_function<D>::value> = valid,
    requires<!std::is_base_of<Base, strip<Ptr>>::value> = valid>
constexpr decltype(auto) _invoke(D Base::*pmd, Ptr&& ptr) noexcept
{
    return (*static_cast<Ptr&&>(ptr)).*pmd;
}

template <typename F, typename... Args>
constexpr decltype(auto) _invoke(F&& f, Args&&... args)
{
    return static_cast<F&&>(f)(static_cast<Args&&>(args)...);
}

template <typename F, typename... Args>
using invoke_t = decltype(_invoke(std::declval<F>(), std::declval<Args>()...));

inline constexpr auto invoke = [](auto&&... args) -> decltype(auto) {
    return detail::_invoke(static_cast<decltype(args)>(args)...);
};
}

#endif
