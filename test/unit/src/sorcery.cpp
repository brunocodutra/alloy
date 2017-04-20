#include <alloy.hpp>

struct add
{
    template<typename... Args>
    constexpr decltype(auto) operator ()(Args&&... args) const {
        return (... + static_cast<Args&&>(args));
    }
};

struct sub
{
    template<typename... Args>
    constexpr decltype(auto) operator ()(Args&&... args) const {
        return (... - static_cast<Args&&>(args));
    }
};

template<typename Lbd>
class lvalue : Lbd
{
public:
    template<typename... Args>
    constexpr decltype(auto) operator ()(Args&&... args) & {
        return Lbd::operator ()(static_cast<Args&&>(args)...);
    }
};

template<typename Lbd>
class rvalue : Lbd
{
public:
    template<typename... Args>
    constexpr decltype(auto) operator ()(Args&&... args) && {
        return Lbd::operator ()(static_cast<Args&&>(args)...);
    }
};

namespace ns {
    struct reg {
        char a;
        int b;
        double c;
    };

    constexpr decltype(auto) size(reg const&) {return alloy::number<3>{};}
    constexpr decltype(auto) at(reg const& r, alloy::number<0>) {return r.a;}
    constexpr decltype(auto) at(reg const& r, alloy::number<1>) {return r.b;}
    constexpr decltype(auto) at(reg const& r, alloy::number<2>) {return r.c;}
}

namespace alloy::detail {
    template<typename X>
    struct wrapped {
        X&& x;
    };

    template<typename X>
    constexpr auto wrap(X&& x) ALLOY_RETURN(
        wrapped<X>{static_cast<X&&>(x)}
    )

    template<typename X>
    constexpr auto unwrap(wrapped<X>&& w) ALLOY_RETURN(
        static_cast<X&&>(w.x)
    )

    template<typename... Args,
        where<(sizeof...(Args) > 0)> = valid
    >
    constexpr auto last(Args&&... args) ALLOY_RETURN(
        unwrap((wrap(args), ...))
    )
}

int main() {
    constexpr ns::reg r{'a', 42, 1.5};
    static_assert(alloy::size(r) == 3);
    static_assert(alloy::at(r)(alloy::number<0>{}) == 'a');
    static_assert(alloy::at(r)(alloy::number<1>{}) == 42);
    static_assert(alloy::at(r)(alloy::number<2>{}) == 1.5);

    static constexpr auto half = .5f;
    static constexpr auto five = 5;
    constexpr auto const x = alloy::tuple<int, double const, decltype((half))>(40, 1.5, half);
    constexpr auto const y = alloy::tuple<long, long long const, decltype((five))>(1L, 3LL, five);

    static_assert(alloy::get<1>(x) == alloy::get<1>(alloy::reverse(x)));

    static_assert(alloy::at(x)(alloy::number<1>{}) == 1.5);
    static_assert(alloy::at(std::move(y))(alloy::number<1>{}) == 3);

    static_assert(alloy::reverse(x) == alloy::make_tuple(0.5f, 1.5, 40));
    static_assert(alloy::reverse(std::move(y)) == alloy::make_tuple(5, 3LL, 1L));

    static_assert(alloy::rotate(x)(alloy::number<-2>{}) == alloy::make_tuple(1.5, 0.5f, 40));
    static_assert(alloy::rotate(x)(alloy::number<-1>{}) == alloy::make_tuple(0.5f, 40, 1.5));
    static_assert(alloy::rotate(x)(alloy::number<0>{}) == alloy::make_tuple(40, 1.5, 0.5f));
    static_assert(alloy::rotate(x)(alloy::number<1>{}) == alloy::make_tuple(1.5, 0.5f, 40));
    static_assert(alloy::rotate(x)(alloy::number<2>{}) == alloy::make_tuple(0.5f, 40, 1.5));

    static_assert(alloy::rotate(std::move(y), alloy::number<-2>{}) == alloy::make_tuple(3LL, 5, 1L));
    static_assert(alloy::rotate(std::move(y), alloy::number<-1>{}) == alloy::make_tuple(5, 1L, 3LL));
    static_assert(alloy::rotate(std::move(y), alloy::number<0>{}) == alloy::make_tuple(1L, 3LL, 5));
    static_assert(alloy::rotate(std::move(y), alloy::number<1>{}) == alloy::make_tuple(3LL, 5, 1L));
    static_assert(alloy::rotate(std::move(y), alloy::number<2>{}) == alloy::make_tuple(5, 1L, 3LL));

    auto f = lvalue<add>{};
    auto g = rvalue<sub>{};

    static_assert(alloy::apply(f)(x) == 42);
    static_assert(alloy::apply(f, std::move(y)) == 9);
    static_assert(alloy::apply(std::move(g))(x) == 38);
    static_assert(alloy::apply(std::move(g), std::move(y)) == -7);

    static_assert(alloy::transform(f)(std::move(y)) == y);
    static_assert(alloy::transform(std::move(g))(x) == x);
    static_assert(alloy::transform(f)(x, std::move(y)) == alloy::make_tuple(41, 4.5, 5.5));
    static_assert(alloy::transform(std::move(g))(x, std::move(y)) == alloy::make_tuple(39, -1.5, -4.5));

    static_assert(alloy::accumulate(f)(0)(std::move(y)) == 9);
    static_assert(alloy::accumulate(std::move(g))(0)(x) == -42);
    static_assert(alloy::accumulate(f)(0)(x, std::move(y)) == 51);
    static_assert(alloy::accumulate(std::move(g))(0)(x, std::move(y)) == -51);

    static_assert(alloy::equal(true, alloy::number<1>{}, metal::number<1>{}));
    static_assert(!alloy::equal(true, alloy::number<1>{}, metal::number<2>{}));

    static_assert(alloy::foldl(f)(40, 1.5, 0.5f) == 42.);
    static_assert(alloy::foldl(std::move(g))(40, 1.5, 0.5f) == 38.);

    static_assert(alloy::foldr(f)(40, 1.5, 0.5f) == 42.);
    static_assert(alloy::foldr(std::move(g))(40, 1.5, 0.5f) == 39.);

    static_assert(alloy::foldl(alloy::make_tuple, 0, 5., 'a') == alloy::make_tuple(alloy::make_tuple(0, 5.), 'a'));
    static_assert(alloy::foldr(alloy::make_tuple, 0, 5., 'a') == alloy::make_tuple(0, alloy::make_tuple(5., 'a')));

    static_assert(alloy::detail::last(0, 5., alloy::make_tuple(), 'a') == 'a');

    return 0;
}

