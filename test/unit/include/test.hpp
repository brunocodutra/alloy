// Copyright Bruno Dutra 2016-2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#ifndef ALLOY_TEST_HPP
#define ALLOY_TEST_HPP

#include <alloy.hpp>
#include <metal.hpp>

using namespace metal;

using _0_ = number<0>;
using _1_ = number<1>;
using _2_ = number<2>;
using _3_ = number<3>;
using _4_ = number<4>;
using _5_ = number<5>;
using _6_ = number<6>;

template<typename... Xs>
struct tester {
    operator int() const {
        return (0 + ... + Xs{});
    }
};

template<template<typename...> class M, typename N = number<10>>
auto test = apply<
    lambda<tester>,
    transform<
        partial<lambda<apply>, lambda<M>>,
        cartesian<iota<_0_, N>, iota<_0_, N>>
    >
>{};

template<typename M, typename N>
struct _nasty;

template<typename N>
struct _nasty<_0_, N> {};

template<typename N>
struct _nasty<_1_, N> {
    struct {} payload = {};
};

template<typename N>
struct _nasty<_2_, N> final {};

template<typename N>
struct _nasty<_3_, N> {
    constexpr _nasty() noexcept(false) {}
    constexpr _nasty(_nasty&&) noexcept(false) {}
    constexpr _nasty(_nasty const&) noexcept(false) {}
};

template<typename N>
struct _nasty<_4_, N> {
    explicit constexpr _nasty() noexcept = default;
    explicit constexpr _nasty(_nasty&&) noexcept = default;
    explicit constexpr _nasty(_nasty const&) noexcept = default;
};

template<typename N>
struct _nasty<_5_, N> {
    constexpr _nasty() noexcept = default;
    constexpr _nasty(_nasty&&) noexcept = default;
    constexpr _nasty(_nasty const&) noexcept = default;

    template<typename... Xs>
    constexpr _nasty(alloy::tuple<Xs...> const&) noexcept {}
};

template<typename M, typename N, typename Arg>
void operator ,(_nasty<M, N> const&, Arg&&) = delete;

template<typename Arg, typename M, typename N>
void operator ,(Arg&&, _nasty<M, N> const&) = delete;

template<typename M, typename N, typename O, typename P>
void operator ,(_nasty<M, N> const&, _nasty<O, P> const&) = delete;

template<typename M, typename N, typename O, typename P>
constexpr auto operator ==(_nasty<M, N> const&, _nasty<O, P> const&) {
    return same<N, P>{};
}

template<typename N>
using nasty = _nasty<mod<N, _6_>, N>;

template<typename M, typename N>
struct _val : _val<mod<M, _6_>, N> {};

template<typename N>
struct _val<_0_, N> {
    static constexpr auto make() {
        return nasty<N>{};
    }
};

template<typename N>
struct _val<_1_, N> {
    static nasty<N> ref;

    static constexpr auto& make() {
        return ref;
    }
};

template<typename N>
nasty<N> _val<_1_, N>::ref{};

template<typename N>
struct _val<_2_, N> {
    static nasty<N> ref;

    static constexpr auto&& make() {
        return std::move(ref);
    }
};

template<typename N>
nasty<N> _val<_2_, N>::ref{};

template<typename N>
struct _val<_3_, N> {
    static constexpr nasty<N> const make() {
        return nasty<N>{};
    }
};

template<typename N>
struct _val<_4_, N> {
    static nasty<N> const ref;

    static constexpr auto& make() {
        return ref;
    }
};

template<typename N>
nasty<N> const _val<_4_, N>::ref{};

template<typename N>
struct _val<_5_, N> {
    static nasty<N> const ref;

    static constexpr auto&& make() {
        return std::move(ref);
    }
};

template<typename N>
nasty<N> const _val<_5_, N>::ref{};

template<typename M, typename N>
constexpr decltype(auto) val() {
    return _val<M, N>::make();
}

template<typename M, typename N>
struct _single : _single<mod<M, _4_>, N> {};

template<typename N>
struct _single<_0_, N> {
    using type = alloy::tuple<decltype(val<_0_, N>())>;

    static constexpr type make() {
        return type{val<_0_, N>()};
    }
};

template<typename N>
struct _single<_1_, N> {
    using type = alloy::tuple<decltype(val<_0_, N>())>;

    static constexpr type const make() {
        return type{val<_0_, N>()};
    }
};

template<typename N>
struct _single<_2_, N> {
    static constexpr alloy::tuple<decltype(val<_0_, N>())> ref{
        val<_0_, N>()
    };

    static constexpr auto& make() {
        return ref;
    }
};

template<typename N>
struct _single<_3_, N> {
    static constexpr alloy::tuple<decltype(val<_0_, N>())> ref{
        val<_0_, N>()
    };

    static constexpr auto&& make() {
        return std::move(ref);
    }
};

template<typename M, typename N>
constexpr decltype(auto) single() {
    return _single<M, N>::make();
}

template<typename M, typename Ns>
struct _many : _many<mod<M, _4_>, Ns> {};

template<typename... Ns>
struct _many<_0_, list<Ns...>> {
    static constexpr auto make() {
        return alloy::tuple<decltype(val<Ns, Ns>())...>{val<Ns, Ns>()...};
    }
};


template<typename... Ns>
struct _many<_1_, list<Ns...>> {
    static constexpr alloy::tuple<decltype(val<Ns, Ns>())...> const make() {
        return alloy::tuple<decltype(val<Ns, Ns>())...>{val<Ns, Ns>()...};
    }
};

template<typename... Ns>
struct _many<_2_, list<Ns...>> {
    static constexpr alloy::tuple<decltype(val<Ns, Ns>())...> ref{
        val<Ns, Ns>()...
    };

    static constexpr auto& make() {
        return ref;
    }
};

template<typename... Ns>
struct _many<_3_, list<Ns...>> {
    static constexpr alloy::tuple<decltype(val<Ns, Ns>())...> ref{
        val<Ns, Ns>()...
    };

    static constexpr auto&& make() {
        return std::move(ref);
    }
};

template<typename M, typename N>
constexpr decltype(auto) many() {
    return _many<M, N>::make();
}

static_assert(std::is_same<decltype(val<_0_, _0_>()), nasty<_0_>>::value);
static_assert(std::is_same<decltype(val<_1_, _0_>()), nasty<_0_>&>::value);
static_assert(std::is_same<decltype(val<_2_, _0_>()), nasty<_0_>&&>::value);
static_assert(std::is_same<decltype(val<_3_, _0_>()), nasty<_0_> const>::value);
static_assert(std::is_same<decltype(val<_4_, _0_>()), nasty<_0_> const&>::value);
static_assert(std::is_same<decltype(val<_5_, _0_>()), nasty<_0_> const&&>::value);

static_assert(std::is_same<decltype(single<_0_, _0_>()), alloy::tuple<nasty<_0_>>>::value);
static_assert(std::is_same<decltype(single<_1_, _0_>()), alloy::tuple<nasty<_0_>> const>::value);
static_assert(std::is_same<decltype(single<_2_, _0_>()), alloy::tuple<nasty<_0_>> const&>::value);
static_assert(std::is_same<decltype(single<_3_, _0_>()), alloy::tuple<nasty<_0_>> const&&>::value);

static_assert(std::is_same<decltype(many<_0_, iota<_0_, _6_>>()), alloy::tuple<nasty<_0_>, nasty<_1_>&, nasty<_2_>&&, nasty<_3_> const, nasty<_4_> const&, nasty<_5_> const&&>>::value);
static_assert(std::is_same<decltype(many<_1_, iota<_0_, _6_>>()), alloy::tuple<nasty<_0_>, nasty<_1_>&, nasty<_2_>&&, nasty<_3_> const, nasty<_4_> const&, nasty<_5_> const&&> const>::value);
static_assert(std::is_same<decltype(many<_2_, iota<_0_, _6_>>()), alloy::tuple<nasty<_0_>, nasty<_1_>&, nasty<_2_>&&, nasty<_3_> const, nasty<_4_> const&, nasty<_5_> const&&> const&>::value);
static_assert(std::is_same<decltype(many<_3_, iota<_0_, _6_>>()), alloy::tuple<nasty<_0_>, nasty<_1_>&, nasty<_2_>&&, nasty<_3_> const, nasty<_4_> const&, nasty<_5_> const&&> const&&>::value);

#endif
