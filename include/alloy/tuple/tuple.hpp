// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_TUPLE_TUPLE_HPP
#define ALLOY_TUPLE_TUPLE_HPP

#include <alloy/config.hpp>

#include <alloy/detail/object.hpp>
#include <alloy/detail/traits.hpp>

namespace alloy::detail {
    template<typename... Xs>
    class tuple : public object_t<Xs...> {
        using base = object_t<Xs...>;

    public:
        template<bool T = true,
            where<std::is_default_constructible<Xs>::value..., T> = valid
        >
        constexpr tuple()
            noexcept(noexcept(base()))
            : base()
        {}

        template<bool T = true,
            where<std::is_convertible<Xs const&, Xs>::value..., T> = valid
        >
        constexpr tuple(Xs const&... xs)
            noexcept(noexcept(base(xs...)))
            : base(xs...)
        {}

        template<bool T = true,
            where<std::is_constructible<Xs, Xs const&>::value..., T> = valid,
            unless<std::is_convertible<Xs const&, Xs>::value..., T> = valid
        >
        explicit constexpr tuple(Xs const&... xs)
            noexcept(noexcept(base(xs...)))
            : base(xs...)
        {}

        template<typename... Ys,
            where<std::is_convertible<Ys&&, Xs>::value...> = valid
        >
        constexpr tuple(Ys&&... ys)
            noexcept(noexcept(base(static_cast<Ys&&>(ys)...)))
            : base(static_cast<Ys&&>(ys)...)
        {}

        template<typename... Ys,
            where<std::is_constructible<Xs, Ys&&>::value...> = valid,
            unless<std::is_convertible<Ys&&, Xs>::value...> = valid
        >
        explicit constexpr tuple(Ys&&... ys)
            noexcept(noexcept(base(static_cast<Ys&&>(ys)...)))
            : base(static_cast<Ys&&>(ys)...)
        {}

        constexpr tuple(tuple&&) = default;
        constexpr tuple(tuple const&) = default;
    };

    template<>
    struct tuple<> : object_t<> {
        constexpr tuple() = default;
        constexpr tuple(tuple&&) = default;
        constexpr tuple(tuple const&) = default;
    };
}

namespace alloy {
    template<typename... Xs>
    using tuple = detail::tuple<Xs...>;
}

#endif
