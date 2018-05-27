#ifndef ALLOY_DETAIL_TRAITS_HPP
#define ALLOY_DETAIL_TRAITS_HPP

#include "../config.hpp"
#include "../external.hpp"

#include <type_traits>

namespace alloy::detail {
    inline constexpr enum class valid_t {} valid = {};

    template<auto cond>
    using requires = std::enable_if_t<cond, valid_t>;

    template<typename T>
    using strip = std::remove_cv_t<std::remove_reference_t<T>>;

    template<typename T, typename U>
    struct _transfer_ref {
        using type = U;
    };

    template<typename T, typename U>
    struct _transfer_ref<T&, U> {
        using type = U&;
    };

    template<typename T, typename U>
    struct _transfer_ref<T&&, U> {
        using type = U&&;
    };

    template<typename T, typename U>
    using transfer_ref = typename _transfer_ref<T, U>::type;

    template<typename T, typename U>
    struct _transfer_cv {
        using type = U;
    };

    template<typename T, typename U>
    struct _transfer_cv<T const, U>
        : _transfer_ref<U, std::remove_reference_t<U> const> {};

    template<typename T, typename U>
    struct _transfer_cv<T volatile, U>
        : _transfer_ref<U, std::remove_reference_t<U> volatile> {};

    template<typename T, typename U>
    struct _transfer_cv<T const volatile, U>
        : _transfer_ref<U, std::remove_reference_t<U> const volatile> {};

    template<typename T, typename U>
    using transfer_cv =
        typename _transfer_cv<std::remove_reference_t<T>, U>::type;

    template<typename T, typename U>
    using transfer = transfer_ref<T, transfer_cv<T, U>>;

    template<typename, template<typename...> class, typename = valid_t>
    struct _instanceof : std::false_type {};

    template<typename... Xs,
        template<typename...>
        class X,
        template<typename...>
        class Y>
    struct _instanceof<X<Xs...>, Y, requires<metal::is_value<Y<Xs...>>::value>>
        : std::is_same<X<Xs...>, Y<Xs...>> {};

    template<typename X, template<typename...> class Tmpl>
    constexpr bool instanceof = _instanceof<strip<X>, Tmpl>::value;

    template<typename X>
    constexpr bool inheritable = std::is_class<X>::value
        && !std::is_polymorphic<X>::value && !std::is_final<X>::value;

    template<typename... Xs>
    using common = metal::fold_left<metal::lambda<std::common_type_t>, Xs...>;
}

#endif
