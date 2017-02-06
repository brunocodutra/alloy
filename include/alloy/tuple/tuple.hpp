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
