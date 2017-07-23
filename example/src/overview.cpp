// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

// NOTE: keep in sync with the README

/// [overview]
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
/// [overview]
