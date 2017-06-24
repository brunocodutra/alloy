// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

#include <alloy.hpp>

#include <tuple>

template<typename>
struct base {};

struct _0 : base<_0> {};
struct _1 : base<_1> {_1(_1&&) = default;};
struct _2 : base<_2> {union x {} x_ = {};};
struct _3 final : base<_3> {};
struct _4 : base<_4> {int payload[1024];};
struct _5 : base<_5> {_5(_5 const&) = delete; _5(_5&&) = default;};
using _6 = base<int>;
struct _7 : base<_7> {virtual void f(){}};
struct _8 : base<_8> {union x{int a; float b;} x_ = {};};

template<typename X>
constexpr bool operator ==(base<X> const&, base<X> const&) {
    return true;
}

template<typename X, typename Y>
constexpr bool operator ==(base<X> const&, base<Y> const&) {
    return false;
}

int main(int /*argc*/, char**) {
    constexpr auto expect = [](auto&&... expected) {
        return [&expected...](auto&&... args) -> bool {
            return (... && (static_cast<decltype(expected)>(expected) == static_cast<decltype(args)>(args)));
        };
    };

    static_assert(expect('a', 42, 0.) << alloy::forward('a', 42, 0.));
    static_assert(expect('a', 42, 0., _0{}, _1{}) << alloy::append(_0{}, _1{}) << alloy::forward('a', 42, 0.));
    static_assert(expect(_0{}, _1{}, 'a', 42, 0.) << alloy::prepend(_0{}, _1{}) << alloy::forward('a', 42, 0.));

    static_assert(expect('a', 42, 0.) << alloy::join(alloy::forward('a'), alloy::capture(42), alloy::forward(0.)));

    static_assert(expect() << alloy::unpack());

    constexpr auto t1 = std::make_tuple(0.);
    constexpr auto t2 = std::make_tuple('a', 42);
    constexpr auto t3 = std::make_tuple('a', 42, 0.);

    static_assert(expect('a', 42, 0.) << alloy::unpack(t3));
    static_assert(expect('a', 42, 0.) << alloy::unpack(t2, t1));

    constexpr std::variant<char, int, double> v1 = 'a';
    constexpr std::variant<char, int, double> v2 = 42;
    constexpr std::variant<char, int, double> v3 = 0.;

    static_assert(expect('a', 42, 0.) << alloy::unpack(v1, v2, v3));

    static_assert(expect('a', 42, 0., 'a', 42, 0., 'a', 42, 0.) << alloy::unpack(v1, v2, t1, t2, v3, t3));

    constexpr auto args = alloy::capture(_0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{});

    static_assert(expect(_1{}, _4{}, _7{}) << alloy::at(1, 4, 7) << args);
    static_assert(expect(_1{}, _4{}, _7{}) << alloy::at(std::true_type{}, alloy::constant<4>{}, metal::number<7>{}) << std::move(args));

    return 0;
}
