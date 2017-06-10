// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt
#ifndef ALLOY_HPP
#define ALLOY_HPP
// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt
#ifndef ALLOY_CONFIG_HPP
#define ALLOY_CONFIG_HPP
// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt
#ifndef ALLOY_CONFIG_VERSION_HPP
#define ALLOY_CONFIG_VERSION_HPP
#define ALLOY_MAJOR 0
#define ALLOY_MINOR 1
#define ALLOY_PATCH 0
#define ALLOY_SEMVER(MAJOR, MINOR, PATCH) (((MAJOR) * 1000000) + ((MINOR) * 10000) + (PATCH))
#define ALLOY_VERSION ALLOY_SEMVER(ALLOY_MAJOR, ALLOY_MINOR, ALLOY_PATCH)
#endif
#endif
// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt
#ifndef ALLOY_ARGS_HPP
#define ALLOY_ARGS_HPP
// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt
#ifndef ALLOY_CONSTANT_HPP
#define ALLOY_CONSTANT_HPP
// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt
#ifndef ALLOY_DETAIL_TRAITS_HPP
#define ALLOY_DETAIL_TRAITS_HPP
// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt
#ifndef ALLOY_EXTERNAL_HPP
#define ALLOY_EXTERNAL_HPP
#if !defined(METAL_VERSION)
#include <metal.hpp>
#endif
#if METAL_VERSION < METAL_SEMVER(0, 6, 0)
#error Unsuported Metal version
#endif
#endif
#include <type_traits>
namespace alloy::detail {
    inline constexpr enum class valid_t {} valid = {};
    template<auto cond>
    using requires = std::enable_if_t<cond, valid_t>;
    template<typename T>
    using strip = std::remove_cv_t<std::remove_reference_t<T>>;
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
    using transfer_cv = typename _transfer_cv<std::remove_reference_t<T>, U>::type;
    template<typename T, typename U>
    using transfer = transfer_cv<T, transfer_ref<T, U>>;
    template<typename, template<typename...> class, typename = valid_t>
    struct _instanceof : std::false_type {};
    template<typename... Xs, template<typename...> class X, template<typename...> class Y>
    struct _instanceof<X<Xs...>, Y, requires<metal::is_value<Y<Xs...>>::value>>
        : std::is_same<X<Xs...>, Y<Xs...>>
    {};
    template<typename X, template<typename...> class Tmpl>
    constexpr bool instanceof = _instanceof<strip<X>, Tmpl>::value;
    template<auto N, typename Outer, typename Inner>
    struct combiner_impl {
        template<typename... Ts>
        using impl = metal::apply<
            Outer,
            metal::transform<
                metal::partial<metal::lambda<metal::apply>, Inner>,
                metal::apply<
                    metal::lambda<metal::cartesian>,
                    metal::transform<
                        metal::always<metal::list<Ts...>>,
                        metal::iota<metal::number<0>, metal::number<N>>
                    >
                >
            >
        >;
        using type = metal::lambda<impl>;
    };
    template<auto N, typename Outer, typename Inner>
    using combiner = typename combiner_impl<N, Outer, Inner>::type;
    template<auto N, typename Outer, typename Inner, typename... Args>
    using combine = metal::invoke<combiner<N, Outer, Inner>, Args...>;
}
#endif
namespace alloy {
    template<auto c>
    using constant = std::integral_constant<decltype(c), c>;
}
#endif
// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt
#ifndef ALLOY_MODEL_HPP
#define ALLOY_MODEL_HPP
// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt
#ifndef ALLOY_DETAIL_FUNCTIONAL_HPP
#define ALLOY_DETAIL_FUNCTIONAL_HPP
#define ALLOY_RETURN(...) -> decltype(__VA_ARGS__) { return __VA_ARGS__; } /**/
namespace alloy::detail {
    template<typename Base, typename F, typename Ref, typename... Args,
        requires<std::is_function_v<F>> = valid,
        requires<std::is_base_of_v<Base, strip<Ref>>> = valid
    >
    constexpr auto invoke(F Base::* pmf, Ref&& ref, Args&&... args) ALLOY_RETURN(
        (static_cast<Ref&&>(ref).*pmf)(static_cast<Args&&>(args)...)
    )
    template<typename Base, typename F, typename Ptr, typename... Args,
        requires<std::is_function_v<F>> = valid,
        requires<!std::is_base_of_v<Base, strip<Ptr>>> = valid
    >
    constexpr auto invoke(F Base::* pmf, Ptr&& ptr, Args&&... args) ALLOY_RETURN(
        ((*static_cast<Ptr&&>(ptr)).*pmf)(static_cast<Args&&>(args)...)
    )
    template<typename Base, typename D, typename Ref,
        requires<!std::is_function_v<D>> = valid,
        requires<std::is_base_of_v<Base, strip<Ref>>> = valid
    >
    constexpr auto invoke(D Base::* pmd, Ref&& ref) noexcept ALLOY_RETURN(
        static_cast<Ref&&>(ref).*pmd
    )
    template<typename Base, typename D, typename Ptr,
        requires<!std::is_function_v<D>> = valid,
        requires<!std::is_base_of_v<Base, strip<Ptr>>> = valid
    >
    constexpr auto invoke(D Base::* pmd, Ptr&& ptr) noexcept ALLOY_RETURN(
        (*static_cast<Ptr&&>(ptr)).*pmd
    )
    template<typename F, typename... Args>
    constexpr auto invoke(F&& f, Args&&... args) ALLOY_RETURN(
        static_cast<F&&>(f)(static_cast<Args&&>(args)...)
    )
    template<typename F, typename... Args>
    using invoke_t = decltype(invoke(std::declval<F>(), std::declval<Args>()...));
    template<typename F, typename = valid_t>
    struct invocable {
        F f;
        constexpr invocable() = delete;
        constexpr invocable(invocable&&) = default;
        constexpr invocable(invocable const&) = default;
        constexpr invocable(F&& f)
            : f{static_cast<F&&>(f)}
        {}
        template<typename... Args>
        constexpr decltype(auto) operator ()(Args&&... args) & {
            return invoke(static_cast<F&>(f), static_cast<Args&&>(args)...);
        }
        template<typename... Args>
        constexpr decltype(auto) operator ()(Args&&... args) const& {
            return invoke(static_cast<F const&>(f), static_cast<Args&&>(args)...);
        }
        template<typename... Args>
        constexpr decltype(auto) operator ()(Args&&... args) && {
            return invoke(static_cast<F&&>(f), static_cast<Args&&>(args)...);
        }
        template<typename... Args>
        constexpr decltype(auto) operator ()(Args&&... args) const&& {
            return invoke(static_cast<F const&&>(f), static_cast<Args&&>(args)...);
        }
    };
    template<typename F>
    struct invocable<F, requires<std::is_class_v<F> && !std::is_final_v<F>>> : F {
        using base = F;
        using base::base;
        constexpr invocable(F&& f)
            : base{static_cast<F&&>(f)}
        {}
    };
    struct nil {};
    template<typename F, typename Arg>
    struct folder {
        F&& f;
        Arg&& arg;
        constexpr F&& operator +() noexcept {
            return static_cast<F&&>(f);
        }
        constexpr Arg&& operator *() noexcept {
            return static_cast<Arg&&>(arg);
        }
        friend constexpr decltype(auto) operator >>=(nil, folder ref) noexcept {
            return *ref;
        }
        template<typename State>
        friend constexpr decltype(auto) operator >>=(State&& state, folder ref) {
            return invoke(+ref, static_cast<State&&>(state), *ref);
        }
        friend constexpr decltype(auto) operator <<=(folder ref, nil) noexcept {
            return *ref;
        }
        template<typename State>
        friend constexpr decltype(auto) operator <<=(folder ref, State&& state) {
            return invoke(+ref, *ref, static_cast<State&&>(state));
        }
    };
    template<typename F, typename... Args>
    constexpr decltype(auto) foldl(F&& f, Args&&... args) {
        return (nil{} >>= ... >>= folder<F, Args>{
            static_cast<F&&>(f),
            static_cast<Args&&>(args)
        });
    }
    template<typename F, typename... Args>
    constexpr decltype(auto) foldr(F&& f, Args&&... args) {
        return (folder<F, Args>{
            static_cast<F&&>(f),
            static_cast<Args&&>(args)
        } <<= ... <<= nil{});
    }
}
#undef ALLOY_RETURN
#endif
namespace alloy {
    template<typename S>
    struct source : detail::invocable<S> {
        using base = detail::invocable<S>;
        using base::base;
        using base::operator ();
    };
    template<typename S>
    source(S&&) -> source<S>;
    template<typename S>
    struct sink : detail::invocable<S> {
        using base = detail::invocable<S>;
        using base::base;
        using base::operator ();
    };
    template<typename S>
    sink(S&&) -> sink<S>;
    template<typename S>
    struct stream : detail::invocable<S> {
        using base = detail::invocable<S>;
        using base::base;
        using base::operator ();
    };
    template<typename S>
    stream(S&&) -> stream<S>;
    template<typename S, typename T,
        detail::requires<detail::instanceof<S, stream>> = detail::valid,
        detail::requires<detail::instanceof<T, stream>> = detail::valid
    >
    constexpr decltype(auto) operator <<(S&& s, T&& t) {
        return stream(
            [&s, &t](auto&& snk) -> decltype(auto) {
                return static_cast<decltype(snk)&&>(snk)
                    << static_cast<S&&>(s)
                    << static_cast<T&&>(t);
            }
        );
    }
    template<typename Stm, typename Src,
        detail::requires<detail::instanceof<Stm, stream>> = detail::valid,
        detail::requires<!detail::instanceof<Src, stream>> = detail::valid
    >
    constexpr decltype(auto) operator <<(Stm&& stm, Src&& src) {
        return source(
            [&stm, &src](auto&& snk) -> decltype(auto) {
                return static_cast<decltype(snk)&&>(snk)
                    << static_cast<Stm&&>(stm)
                    << static_cast<Src&&>(src);
            }
        );
    }
    template<typename Snk, typename Stm,
        detail::requires<!detail::instanceof<Snk, stream>> = detail::valid,
        detail::requires<detail::instanceof<Stm, stream>> = detail::valid
    >
    constexpr decltype(auto) operator <<(Snk&& snk, Stm&& stm) {
        return sink(invoke(static_cast<Stm&&>(stm), static_cast<Snk&&>(snk)));
    }
    template<typename Snk, typename Src,
        detail::requires<
            detail::instanceof<Snk, sink> ||
            detail::instanceof<Src, source>
        > = detail::valid
    >
    constexpr decltype(auto) operator <<(Snk&& snk, Src&& src) {
        return invoke(static_cast<Src&&>(src), static_cast<Snk&&>(snk));
    }
}
#endif
namespace alloy::detail {
    template<typename K, typename V = K>
    struct arg {
        V v;
        using type = V;
        constexpr arg() = default;
        constexpr arg(arg&&) = default;
        constexpr arg(arg const&) = default;
        constexpr arg(V&& v) noexcept
            : v(static_cast<V&&>(v))
        {}
        constexpr operator transfer<arg&, V>() & noexcept {
            return static_cast<transfer<arg&, V>>(v);
        }
        constexpr operator transfer<arg const&, V>() const& noexcept {
            return static_cast<transfer<arg const&, V>>(v);
        }
        constexpr operator transfer<arg&&, V>() && noexcept {
            return static_cast<transfer<arg&&, V>>(v);
        }
        constexpr operator transfer<arg const&&, V>() const&& noexcept {
            return static_cast<transfer<arg const&&, V>>(v);
        }
    };
    template<typename...>
    struct args;
    template<typename... Ks, typename... Vs>
    struct args<arg<Ks, Vs>...> : arg<Ks, Vs>... {
        constexpr args() = default;
        constexpr args(args&&) = default;
        constexpr args(args const&) = default;
        constexpr args(Vs&&... vs)
            : arg<Ks, Vs>(static_cast<Vs&&>(vs))...
        {}
        template<auto i,
            typename Arg = metal::at<metal::as_list<args>, metal::number<i>>
        >
        constexpr decltype(auto) operator [](constant<i>) & noexcept {
            return args::at<Arg>(static_cast<args&>(*this));
        }
        template<auto i,
            typename Arg = metal::at<metal::as_list<args>, metal::number<i>>
        >
        constexpr decltype(auto) operator [](constant<i>) const& noexcept {
            return args::at<Arg>(static_cast<args const&>(*this));
        }
        template<auto i,
            typename Arg = metal::at<metal::as_list<args>, metal::number<i>>
        >
        constexpr decltype(auto) operator [](constant<i>) && noexcept {
            return args::at<Arg>(static_cast<args&&>(*this));
        }
        template<auto i,
            typename Arg = metal::at<metal::as_list<args>, metal::number<i>>
        >
        constexpr decltype(auto) operator [](constant<i>) const&& noexcept {
            return args::at<Arg>(static_cast<args const&&>(*this));
        }
        template<typename F>
        constexpr decltype(auto) operator ()(F&& f) & {
            return args::call(
                static_cast<args&>(*this),
                static_cast<F&&>(f)
            );
        }
        template<typename F>
        constexpr decltype(auto) operator ()(F&& f) const& {
            return args::call(
                static_cast<args const&>(*this),
                static_cast<F&&>(f)
            );
        }
        template<typename F>
        constexpr decltype(auto) operator ()(F&& f) && {
            return args::call(
                static_cast<args&&>(*this),
                static_cast<F&&>(f)
            );
        }
        template<typename F>
        constexpr decltype(auto) operator ()(F&& f) const&& {
            return args::call(
                static_cast<args const&&>(*this),
                static_cast<F&&>(f)
            );
        }
    private:
        template<typename Arg, typename Self>
        static constexpr decltype(auto) at(Self&& self) noexcept {
            return static_cast<transfer<Self&&, typename Arg::type>>(
                static_cast<transfer<Self&&, Arg>>(
                    static_cast<Self&&>(self)
                )
            );
        }
        template<typename Self, typename F>
        static constexpr decltype(auto) call(Self&& self, F&& f) {
            return invoke(
                static_cast<F&&>(f),
                args::at<arg<Ks, Vs>>(static_cast<Self&&>(self))...
            );
        }
    };
    template<typename... Vs>
    using args_t = metal::apply<
        metal::lambda<args>,
        metal::transform<
            metal::lambda<arg>,
            metal::indices<metal::list<Vs...>>,
            metal::list<Vs...>
        >
    >;
}
namespace alloy {
    template<typename... Vs>
    constexpr decltype(auto) forward(Vs&&... vs) noexcept {
        return source(detail::args_t<Vs&&...>{static_cast<Vs&&>(vs)...});
    }
    template<typename... Vs>
    constexpr decltype(auto) capture(Vs&&... vs) {
        return source(detail::args_t<Vs...>{static_cast<Vs&&>(vs)...});
    }
}
#endif
// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt
#ifndef ALLOY_AT_HPP
#define ALLOY_AT_HPP
namespace alloy::detail {
    template<typename... Is>
    struct sampler {
        template<typename R, typename F, typename... Args>
        static constexpr R dispatch(F&& snk, Args&&... args) {
            return invoke(
                static_cast<F&&>(snk),
                forward(static_cast<Args&&>(args)...)[Is{}]...
            );
        }
    };
    template<typename... Samplers>
    struct samplers {
        template<typename R, typename F, typename... Args>
        static constexpr R dispatch(std::size_t i, F&& f, Args&&... args) {
            using Sig = R(F&&, Args&&...);
            constexpr Sig* dispatchers[] = {
                &Samplers::template dispatch<R, F, Args...>...
            };
            return dispatchers[i](
                static_cast<F&&>(f),
                static_cast<Args&&>(args)...
            );
        }
    };
    template<typename... Is>
    constexpr auto at(Is&&... is) noexcept {
        return [&is...](auto&& snk) noexcept {
            return [&is..., &snk](auto&&... args) -> decltype(auto) {
                using R = combine<
                    sizeof...(Is),
                    metal::lambda<std::common_type_t>,
                    metal::partial<metal::lambda<invoke_t>, decltype(snk)>,
                    decltype(args)...
                >;
                using Samplers = metal::apply<
                    combiner<
                        sizeof...(is),
                        metal::lambda<samplers>,
                        metal::lambda<sampler>
                    >,
                    metal::indices<metal::list<decltype(args)...>>
                >;
                return Samplers::template dispatch<R>(
                    foldr([](auto&& i, std::size_t j) {
                        return static_cast<decltype(i)&&>(i) + sizeof...(args)*j;
                    }, static_cast<Is&&>(is)...),
                    static_cast<decltype(snk)&&>(snk),
                    static_cast<decltype(args)&&>(args)...
                );
            };
        };
    }
    template<auto... is>
    constexpr auto at(constant<is>...) noexcept {
        return [](auto&& snk) noexcept {
            return [&snk](auto&&... args) -> decltype(auto) {
                using Args = metal::list<decltype(args)...>;
                using R = invoke_t<
                    decltype(snk),
                    metal::at<Args, metal::number<is>>...
                >;
                using Sampler = sampler<constant<is>...>;
                return Sampler::template dispatch<R>(
                    static_cast<decltype(snk)&&>(snk),
                    static_cast<decltype(args)&&>(args)...
                );
            };
        };
    }
    constexpr auto at() noexcept {
        return [](auto&& snk) noexcept {
            return [&snk](auto&&...) -> decltype(auto) {
                return invoke(static_cast<decltype(snk)&&>(snk));
            };
        };
    }
}
namespace alloy {
    inline constexpr auto at = [](auto&&... is) {
        return stream(detail::at(static_cast<decltype(is)&&>(is)...));
    };
}
#endif
#endif
