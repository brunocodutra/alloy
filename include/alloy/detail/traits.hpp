// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_DETAIL_TRAITS_HPP
#define ALLOY_DETAIL_TRAITS_HPP

#include <alloy/config.hpp>

#include <type_traits>

namespace alloy::detail {
    template<typename T>
    struct _strip_ref {using type = T;};

    template<typename T>
    struct _strip_ref<T&> {using type = T;};

    template<typename T>
    struct _strip_ref<T&&> {using type = T;};

    template<typename T>
    using strip_ref = typename _strip_ref<T>::type;

    template<typename T>
    struct _strip_cv {using type = T;};

    template<typename T>
    struct _strip_cv<T const> {using type = T;};

    template<typename T>
    struct _strip_cv<T volatile> {using type = T;};

    template<typename T>
    struct _strip_cv<T const volatile> {using type = T;};

    template<typename T>
    using strip_cv = typename _strip_cv<T>::type;

    template<typename T>
    using strip = strip_cv<strip_ref<T>>;

    template<typename T, typename U>
    struct _transfer_ref {using type = U;};

    template<typename T, typename U>
    struct _transfer_ref<T&, U> {using type = U&;};

    template<typename T, typename U>
    struct _transfer_ref<T&, U&&> {using type = U&&;};

    template<typename T, typename U>
    struct _transfer_ref<T&&, U> {using type = U&&;};

    template<typename T, typename U>
    using transfer_ref = typename _transfer_ref<T, U>::type;

    template<typename T, typename U>
    struct _transfer_cv {using type = U;};

    template<typename T, typename U>
    struct _transfer_cv<T const, U&> {using type = U const&;};

    template<typename T, typename U>
    struct _transfer_cv<T const, U&&> {using type = U const&&;};

    template<typename T, typename U>
    struct _transfer_cv<T volatile, U&> {using type = U volatile&;};

    template<typename T, typename U>
    struct _transfer_cv<T volatile, U&&> {using type = U volatile&&;};

    template<typename T, typename U>
    struct _transfer_cv<T const volatile, U&> {using type = U const volatile&;};

    template<typename T, typename U>
    struct _transfer_cv<T const volatile, U&&> {using type = U const volatile&&;};

    template<typename T, typename U>
    using transfer_cv = typename _transfer_cv<strip_ref<T>, U>::type;

    template<typename T, typename U>
    using transfer = transfer_cv<T, transfer_ref<T, U>>;
}

#endif
