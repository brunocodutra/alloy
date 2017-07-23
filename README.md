# Alloy
Enjoy (post) modern C++

[![version]][semver]
[![travis.badge]][travis.alloy]
[![gitter.badge]][gitter.alloy]

## Overview

**Alloy** solves the problem of run-time manipulation of heterogeneous values
with more flexibility and expressive power than the tools provided by the
standard library.

```.cpp
#include "alloy.hpp"

#include <iostream>
#include <utility>

int main() {
    // suppose you have some data
    auto data = alloy::capture("World", ' ', "Hello" , '!');

    // and means to consume it
    auto print = [](auto&&... data) {
        // we'll simply print to the standard output in this example
        (std::cout << ... << std::forward<decltype(data)>(data)) << std::endl;
    };

    // all you need is to connect the ends
    print << data; // prints World Hello!

    // that's not all, you can add filters in between
    print << alloy::at(2, 1, 0, 3) << data; // prints Hello World!

    // and also chain several of them
    print << alloy::append('C', '+', '+', '!')
          << alloy::append(" (post) modern ")
          << alloy::at(2)
          << data; // prints Hello (post) modern C++!

    // there's more, with Alloy you can kiss `std::apply` goodbye
    print << alloy::unpack(std::make_tuple(3, '.', 1, 4)); // prints 3.14

    // ever wanted to iterate through a `std::tuple` in a for-loop?
    constexpr auto tup = std::make_tuple(0, '1', "two");

    // yes you can!
    for(std::size_t i = 0; i < std::tuple_size<decltype(tup)>{}; ++i)
        print << alloy::prepend(i, ": ") << alloy::at(i) << alloy::unpack(tup);

    // prints
    // 0: 0
    // 1: 1
    // 2: two

    // `std::visit` is also a thing of the past
    using var = std::variant<int, char, char const*>;

    var i = 3;
    var c = '.';
    var s = "1";

    print << alloy::unpack(i, c, s); // prints 3.1

    // while you are at it, why not mixing tuples and variants together?
    print << alloy::unpack(i, c, s, std::make_tuple(4, "15")); // prints 3.1415

    // tuples and variants are too mainstream?
    // not a problem, you can also provide your very own custom data sources
    auto produce = [](auto&& consume) {
        return consume("Hello", ' ', "World");
    };

    // the following are all equivalent and print Hello World
    print << alloy::source{produce};
    alloy::sink{print} << produce;
    alloy::sink{print} << alloy::source{produce};

    // and your very own custom filters too
    auto process = [](auto&& sink) {
        return [&sink](auto&& hello, auto&& _, auto&& world) {
            return sink(hello, _, "brave", _, "new", _, world, '!');
        };
    };

    print << alloy::filter{process} << produce; // prints Hello brave new World!

    // enjoy (post) modern C++

    auto wrap = [](auto&& sink) {
        return [&sink](auto&& word) {
            return sink('(', word, ')');
        };
    };

    print << alloy::append(' ', "modern C++")
          << alloy::prepend("enjoy", ' ')
          << wrap
          << alloy::forward("post");
}
```

[Try it live on Wandbox][wandbox]

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
decltype(auto) copy_if(Tuple&&, Predicate&&);

auto x = copy_if(std::make_tuple(1, 1.0, '1', "one"), [](auto&&) {
    return std::rand() % 2;
});
```

Heck, the predicate doesn't even depend on the elements themselves, but still we
are unable to deduce the return type!

Alright, forget about returning values, let's forward results to a callback
function instead.

```.cpp
template<typename Tuple, typename Predicate, typename Callback>
void copy_if(Tuple&&, Predicate&&, Callback&&);

auto predicate = [](auto&&) {
    return std::rand() % 2;
};

auto callback = [](auto&&... /*args*/) { /* ... */ };

copy_if(std::make_tuple(1, 1.0, '1', "one"), predicate, callback);
```

That was easy after all... or was it? Let us not forget that we still need to
implement `copy_if`.

How can the standard library help us get there, you might have asked yourself,
and the answer is quite simple in fact: _It can't really._

Sure `std::apply` can help us extract the elements out of the tuple, but that's
about all the standard library can do for us, from then on we are on our own.

```.cpp
template<typename Tuple, typename Predicate, typename Callback>
void copy_if(Tuple&& tuple, Predicate&& predicate, Callback&& callback) {
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

callback << alloy::copy_if(predicate) << alloy::unpack(tuple);
```

We need Alloy.

## Portability

The following compilers are continuously tested on [Travis CI][travis.alloy].

| Compiler          | Version   | Operating System
|-------------------|-----------|-----------------------
| GCC               | 7         | Ubuntu 14.04 LTS
| Clang             | 5.0       | Ubuntu 14.04 LTS

## License

Alloy is distributed under the
[Boost Software License, Version 1.0][boost.license].

[version]:          http://badge.fury.io/gh/brunocodutra%2Falloy.svg
[semver]:           http://semver.org

[travis.alloy]:     http://travis-ci.org/brunocodutra/alloy
[travis.badge]:     http://travis-ci.org/brunocodutra/alloy.svg?branch=master

[gitter.alloy]:     https://gitter.im/brunocodutra/alloy?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge
[gitter.badge]:     https://badges.gitter.im/brunocodutra/alloy.svg

[boost.license]:    http://boost.org/LICENSE_1_0.txt

[wandbox]:          http://wandbox.org/permlink/4tx6EsfjKQzAy8Wl
