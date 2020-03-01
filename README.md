# Alloy [![version]][semver] [![gitter.badge]][gitter.alloy] [![godbolt.badge]][godbolt.alloy]
Embrace (post) modern C++

## Overview

**Alloy** solves the problem of run-time manipulation of heterogeneous values
with more flexibility and expressive power than the tools provided by the
standard library.

```.cpp
#include "alloy.hpp"

#include <iostream>
#include <string>
#include <utility>

int main() {
    // suppose you have some data
    auto data = alloy::capture("Hello", ' ', "World" , '!');

    // and means to consume it
    auto print = [](auto&&... data) {
        // we'll simply print to the standard output in this example
        (std::cout << ... << std::forward<decltype(data)>(data)) << std::endl;
    };

    // all you need is to connect the ends
    data >> print; // Hello World!

    // just as easily you can build data pipelines
    auto predicate = [](auto x) {
        return std::string{x} != "!";
    };

    data >> alloy::filter(predicate) >> print; // Hello World

    // with Alloy you can kiss `std::apply` goodbye
    alloy::unpack(std::make_tuple(3, '.', "14")) >> print; // 3.14

    // you can even iterate through tuples in a regular for-loop
    auto tup = std::make_tuple(3, '.', "14");

    for(std::size_t i = 0; i < std::tuple_size<decltype(tup)>{}; ++i)
        alloy::unpack(tup) >> alloy::at(i) >> alloy::prepend(i, ": ") >> print;

    // 0: 3
    // 1: .
    // 2: 14

    // `std::visit` is also a thing of the past
    using var = std::variant<int, char, std::string>;

    var i = 3;
    var c = '.';
    var s = "14";

    alloy::unpack(i, c, s) >> print; // 3.14

    // while you are at it, why not mixing tuples and variants together?
    alloy::unpack(std::make_tuple("pi", '='), i, c, s) >> print; // pi=3.14

    // tuples and variants are too mainstream?
    // not a problem, you can also provide your very own custom data sources
    auto produce = [](auto consume) {
        return consume("Hello", ' ', "World");
    };

    // the following are all equivalent and print Hello World
    alloy::source{produce} >> print;
    produce >> alloy::sink{print};
    alloy::source{produce} >> alloy::sink{print};

    // and your very own streams too
    auto process = [](auto const& sink) {
        return [&sink](auto hello, auto _, auto world) {
            return sink(hello, _, "brave", _, "new", _, world, '!');
        };
    };

    produce >> alloy::stream{process} >> print; // Hello brave new World!

    // embrace (post) modern C++

    auto wrap = [](auto const& sink) {
        return [&sink](auto word) {
            return sink('(', word, ')');
        };
    };

    alloy::forward("post") >> alloy::stream{wrap}
                           >> alloy::prepend("embrace", ' ')
                           >> alloy::append(' ', "modern C++") >> print;
}
```

## Motivation

Have you ever wished `std::tuple` provided an overload for `operator []` just
like`std::vector` and `std::map` do?

As you might have guessed, it's actually impossible to overload the subscript
operator for tuples, but the reason why might not be obvious at first.

An example is worth a thousand words, so can you guess the type of `x` below?

```.cpp
auto x = std::make_tuple(1, 1.0, '1', "one")[std::rand() % 4];
```

The type of `x` depends on a run-time value and therefore may not be deduced by
the compiler, which is thus unable to compile such a program to begin with.

Fine, as long as we are able to, say, filter the elements in a `std::tuple`
based on a predicate, we should be able to tackle most real world problems.
That should be easy right?

Nope.

Can you deduce the type of `x` this time?

```.cpp
template<typename Tuple, typename Predicate>
decltype(auto) filter(Tuple&&, Predicate&&);

auto x = filter(std::make_tuple(1, 1.0, '1', "one"), [](auto&&) {
    return std::rand() % 2;
});
```

Heck, the predicate doesn't even depend on the elements themselves, but still we
are unable to deduce the return type!

Alright, forget about returning values, let's forward results to a callback
function instead.

```.cpp
template<typename Tuple, typename Predicate, typename Callback>
void filter(Tuple&&, Predicate&&, Callback&&);

auto predicate = [](auto&&) {
    return std::rand() % 2;
};

auto callback = [](auto&&... /*args*/) { /* ... */ };

filter(std::make_tuple(1, 1.0, '1', "one"), predicate, callback);
```

That was easy after all... or was it? Let us not forget that we still need to
implement `filter`.

How can the standard library help us get there, you might have asked yourself,
and the answer is quite simple in fact: _It can't really._

Sure `std::apply` can help us extract the elements out of the tuple, but that's
about all the standard library can do for us, from then on we are on our own.

```.cpp
template<typename Tuple, typename Predicate, typename Callback>
void filter(Tuple&& tuple, Predicate&& predicate, Callback&& callback) {
    constexpr auto impl = [&predicate, &callback](auto&&... elements) {
        // TODO: do the heavy lifting :(
    };

    std::apply(std::forward<Tuple>(tuple), impl);
}
```

We don't want to keep reinventing the wheel, we want to solve real problems, but
for that we need the building blocks.

```.cpp
auto tuple = std::make_tuple(1, 1.0, '1', "one");

auto predicate = [](auto&&) {
    return std::rand() % 2;
};

auto callback = [](auto&&... /*args*/) { /* ... */ };

alloy::unpack(tuple) >> alloy::filter(predicate) >> callback;
```

We need Alloy.

## Quick Start

1. Download [alloy.hpp][releases]
2. `# include </path/to/alloy.hpp>`
3. Embrace (post) modern C++

## Portability

Alloy requires C++17 and is known to work on GCC 7+ and Clang 5+.

## License

This project is licensed under the [MIT][license].

[version]:          https://badge.fury.io/gh/brunocodutra%2Falloy.svg
[semver]:           https://semver.org

[gitter.alloy]:     https://gitter.im/brunocodutra/alloy?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge
[gitter.badge]:     https://badges.gitter.im/brunocodutra/alloy.svg

[godbolt.alloy]:    https://godbolt.org/z/SfXD6H
[godbolt.badge]:    https://img.shields.io/badge/try%20it-on%20godbolt-222266.svg

[license]:          https://github.com/brunocodutra/alloy/blob/master/LICENSE
[releases]:         https://github.com/brunocodutra/alloy/releases
