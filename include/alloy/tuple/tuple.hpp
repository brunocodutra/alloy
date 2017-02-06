// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_TUPLE_TUPLE_HPP
#define ALLOY_TUPLE_TUPLE_HPP

#include <alloy/config.hpp>

#include <alloy/detail/lookup.hpp>
#include <alloy/detail/object.hpp>
#include <alloy/detail/traits.hpp>

#include <utility>

namespace alloy::detail {
    template<typename... Xs>
    class tuple : public object_t<Xs...> {
        using base = object_t<Xs...>;

        struct _ {};

        template<typename... Props, typename T>
        explicit constexpr tuple(_, metal::list<Props...>, T&& t)
            noexcept(noexcept(base(ALLOY_LOOKUP(Props, T&&, t)...)))
            : base(ALLOY_LOOKUP(Props, T&&, t)...)
        {}

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

        template<typename... Ys,
            typename props = props<tuple<Ys...>>,
            where<std::is_convertible<Ys const&, Xs>::value...> = valid
        >
        constexpr tuple(tuple<Ys...> const& ys)
            noexcept(noexcept(tuple(_{}, props{}, ys)))
            : tuple(_{}, props{}, ys)
        {}

        template<typename... Ys,
            typename props = props<tuple<Ys...>>,
            where<std::is_constructible<Xs, Ys const&>::value...> = valid,
            unless<std::is_convertible<Ys const&, Xs>::value...> = valid
        >
        explicit constexpr tuple(tuple<Ys...> const& ys)
            noexcept(noexcept(tuple(_{}, props{}, ys)))
            : tuple(_{}, props{}, ys)
        {}

        template<typename... Ys,
            typename props = props<tuple<Ys...>>,
            where<std::is_convertible<Ys&&, Xs>::value...> = valid
        >
        constexpr tuple(tuple<Ys...>&& ys)
            noexcept(noexcept(tuple(_{}, props{}, std::move(ys))))
            : tuple(_{}, props{}, std::move(ys))
        {}

        template<typename... Ys,
            typename props = props<tuple<Ys...>>,
            where<std::is_constructible<Xs, Ys&&>::value...> = valid,
            unless<std::is_convertible<Ys&&, Xs>::value...> = valid
        >
        explicit constexpr tuple(tuple<Ys...>&& ys)
            noexcept(noexcept(tuple(_{}, props{}, std::move(ys))))
            : tuple(_{}, props{}, std::move(ys))
        {}

        constexpr tuple(tuple&&) = default;
        constexpr tuple(tuple const&) = default;
    };

    template<typename X>
    class tuple<X> : public object_t<X> {
        using base = object_t<X>;

    public:
        template<bool T = true,
            where<std::is_default_constructible<X>::value, T> = valid
        >
        constexpr tuple()
            noexcept(noexcept(base()))
            : base()
        {}

        template<bool T = true,
            where<std::is_convertible<X const&, X>::value, T> = valid
        >
        constexpr tuple(X const& x)
            noexcept(noexcept(base(x)))
            : base(x)
        {}

        template<bool T = true,
            where<std::is_constructible<X, X const&>::value, T> = valid,
            unless<std::is_convertible<X const&, X>::value, T> = valid
        >
        explicit constexpr tuple(X const& x)
            noexcept(noexcept(base(x)))
            : base(x)
        {}

        template<typename Y,
            where<std::is_convertible<Y&&, X>::value> = valid
        >
        constexpr tuple(Y&& y)
            noexcept(noexcept(base(static_cast<Y&&>(y))))
            : base(static_cast<Y&&>(y))
        {}

        template<typename Y,
            where<std::is_constructible<X, Y&&>::value> = valid,
            unless<std::is_convertible<Y&&, X>::value> = valid
        >
        explicit constexpr tuple(Y&& y)
            noexcept(noexcept(base(static_cast<Y&&>(y))))
            : base(static_cast<Y&&>(y))
        {}

        template<typename Y,
            typename prop = prop<0U, Y>,
            where<std::is_convertible<Y const&, X>::value> = valid,
            unless<std::is_convertible<tuple<Y> const&, X>::value> = valid,
            unless<std::is_same<X, Y>::value> = valid
        >
        constexpr tuple(tuple<Y> const& y)
            noexcept(noexcept(base(ALLOY_LOOKUP(prop, tuple<Y> const&, y))))
            : base(ALLOY_LOOKUP(prop, tuple<Y> const&, y))
        {}

        template<typename Y,
            typename prop = prop<0U, Y>,
            where<std::is_constructible<X, Y const&>::value> = valid,
            unless<std::is_convertible<Y const&, X>::value> = valid,
            unless<std::is_constructible<X, tuple<Y> const&>::value> = valid,
            unless<std::is_same<X, Y>::value> = valid
        >
        explicit constexpr tuple(tuple<Y> const& y)
            noexcept(noexcept(base(ALLOY_LOOKUP(prop, tuple<Y> const&, y))))
            : base(ALLOY_LOOKUP(prop, tuple<Y> const&, y))
        {}

        template<typename Y,
            typename prop = prop<0U, Y>,
            where<std::is_convertible<Y&&, X>::value> = valid,
            unless<std::is_convertible<tuple<Y>, X>::value> = valid,
            unless<std::is_same<X, Y>::value> = valid
        >
        constexpr tuple(tuple<Y>&& y)
            noexcept(noexcept(base(ALLOY_LOOKUP(prop, tuple<Y>&&, y))))
            : base(ALLOY_LOOKUP(prop, tuple<Y>&&, y))
        {}

        template<typename Y,
            typename prop = prop<0U, Y>,
            where<std::is_constructible<X, Y&&>::value> = valid,
            unless<std::is_convertible<Y&&, X>::value> = valid,
            unless<std::is_constructible<X, tuple<Y>>::value> = valid,
            unless<std::is_same<X, Y>::value> = valid
        >
        explicit constexpr tuple(tuple<Y>&& y)
            noexcept(noexcept(base(ALLOY_LOOKUP(prop, tuple<Y>&&, y))))
            : base(ALLOY_LOOKUP(prop, tuple<Y>&&, y))
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
