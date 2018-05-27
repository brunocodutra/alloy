#ifndef ALLOY_TEST_HPP
#define ALLOY_TEST_HPP

#define FWD(...) static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__)

template<typename... Matrix>
struct tester : Matrix... {
    constexpr operator int() const noexcept {
        return 0;
    }
};

template<template<int...> class matrix>
struct _lift {
    template<typename... Ns>
    using impl = matrix<Ns::value...>;

    using type = metal::lambda<impl>;
};

template<template<int...> class matrix>
using lift = typename _lift<matrix>::type;

inline constexpr metal::int_ INF = 4;

template<template<int...> class matrix>
constexpr auto test = metal::cascade<
    metal::combine<
        metal::iota<metal::number<0>, metal::number<INF>>,
        metal::number<3>
    >,
    metal::lambda<tester>,
    lift<matrix>
>{};

template<int, typename T>
struct nasty;

template<typename T>
struct nasty<0, T> : T {
    explicit constexpr nasty() noexcept = default;
    explicit constexpr nasty(nasty&&) noexcept = default;
    explicit constexpr nasty(nasty const&) noexcept = default;
};

template<typename T>
struct nasty<1, T> : T {
    constexpr nasty() noexcept(false) {}
    constexpr nasty(nasty&&) noexcept(false) {}
    constexpr nasty(nasty const&) noexcept(false) {}
    constexpr nasty(nasty const&&) noexcept(false) {};

    template<typename... Xs>
    constexpr nasty(Xs&&...) = delete;
};

template<typename T>
struct nasty<2, T> final : nasty<0, T> {
    using base = nasty<0, T>;
    using base::base;
};

template<typename T>
struct nasty<3, T> : nasty<1, T> {
    using base = nasty<1, T>;
    using base::base;

    virtual void polymorphic() {}
};

template<typename T>
inline std::remove_reference_t<T> instance{};

template<int X, typename T>
constexpr decltype(auto) qualify(T&& t = static_cast<T&&>(instance<T>)) {
    if /****/ constexpr(X == 0) {
        return static_cast<T&>(t);
    } else if constexpr(X == 1) {
        return static_cast<T&&>(t);
    } else if constexpr(X == 2) {
        return static_cast<T const&>(t);
    } else if constexpr(X == 3) {
        return static_cast<T const&&>(t);
    }
}

template<int X>
struct value_t {
    template<typename I>
    constexpr auto operator<(I const& i) const noexcept {
        using namespace metal;
        if constexpr(is_invocable<lambda<as_number>, I>{}) {
            return less<number<X>, as_number<I>>{};
        } else {
            return X < i;
        }
    }
};

template<int X, int Y, int Z>
constexpr decltype(auto) value() {
    return qualify<X, nasty<Y, value_t<Z>>>();
}

template<int Y, typename F>
struct callable_t;

template<typename F>
struct callable_t<0, F> {
    F f;

    template<typename... Args>
    constexpr decltype(auto) operator()(Args&&... args) & {
        return f(FWD(args)...);
    }
};

template<typename F>
struct callable_t<1, F> {
    F f;

    template<typename... Args>
    constexpr decltype(auto) operator()(Args&&... args) && {
        return f(FWD(args)...);
    }
};

template<typename F>
struct callable_t<2, F> {
    F f;

    template<typename... Args>
    constexpr decltype(auto) operator()(Args&&... args) const& {
        return f(FWD(args)...);
    }
};

template<typename F>
struct callable_t<3, F> {
    F f;

    template<typename... Args>
    constexpr decltype(auto) operator()(Args&&... args) const&& {
        return f(FWD(args)...);
    }
};

template<int X, typename F>
constexpr auto callable(F&& f) {
    return callable_t<X, F>{FWD(f)};
}

template<int X, int Y, int... Zs>
struct values_t {
    template<typename F>
    constexpr decltype(auto) operator()(F&& f) const {
        return FWD(f)(value<X, Y, Zs>()...);
    }
};

template<int X, int Y, int Z>
constexpr decltype(auto) values() {
    using namespace metal;
    using v = partial<lift<values_t>, number<X>, number<Y>>;
    using is = iota<number<(Z > 0) ? 0 : - 1 - Z>, number<Z>>;
    return qualify<X, nasty<Y, callable_t<X, apply<v, is>>>>();
}

inline constexpr struct {
    template<typename... Ts>
    constexpr auto operator()(Ts&&...) const {
        struct _ {};
        return  _{};
    }
} sink = {};

template<typename T>
using is_source_impl =
    metal::same<decltype(std::declval<T>()(sink)), decltype(sink())>;

template<typename T>
using is_source = metal::is_invocable<metal::lambda<is_source_impl>, T>;

constexpr auto cat() {
    return [](auto&& f) {
        return FWD(f)();
    };
}

template<typename Head>
constexpr decltype(auto) cat(Head&& head) {
    if constexpr(is_source<Head>{}) {
        return FWD(head);
    } else {
        return [&head](auto&& f) {
            return FWD(f)(FWD(head));
        };
    }
}

template<typename Head, typename... Tail>
constexpr auto cat(Head&& head, Tail&&... tail) {
    return [&head, &tail...](auto&& f) {
        return cat(FWD(head))([&f, &tail...](auto&&... xs) {
            return cat(FWD(tail)...)([&f, &xs...](auto&&... ys) {
                return FWD(f)(FWD(xs)..., FWD(ys)...);
            });
        });
    };
}

template<typename... Expected>
constexpr auto expect(Expected&&... expected) {
    return [&expected...](auto&&... rs) {
        return cat(FWD(expected)...)([](auto&&... es) {
            using namespace metal;
            return std::is_same_v<list<decltype(rs)...>, list<decltype(es)...>>;
        });
    };
}

template<typename T>
constexpr T const& copy(T const& t) noexcept {
    return t;
}

#endif


