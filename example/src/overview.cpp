// Copyright Bruno Dutra 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt

// NOTE: keep in sync with the README

/// [overview]
#include "alloy.hpp"

#include <iostream>
#include <utility>
#include <string_view>

int main() {
    using namespace  std::literals::string_view_literals;

    // suppose you have a routine that consumes data
    constexpr auto print = [](auto&&... data) {
        // in this example we'll simply print to the standard output
        (std::cout << ... << std::forward<decltype(data)>(data)) << std::endl;
    };

    // you just have to produce data
    print << alloy::forward("The answer is: ", 42); // prints The answer is 42

    // you can also just as easily store data to be consumed later
    constexpr auto data = alloy::capture(42, ' ', "Hello World"sv, '!');

    print << data; // prints 42 Hello World!

    // you can also process data before it's consumed
    // here we cherry -ick individual elements
    print << alloy::at(2, 3) << data; // prints Hello World!

    // that's right, you can use runtime indices with Alloy!!

    // you can even pick the same element multiple times
    print << alloy::at(2, 3, 3) << data; // prints Hello World!!

    // or none at all
    print << alloy::at() << data; // prints an empty line

    // you also build a pipeline out of data processors
    print << alloy::prepend("Say", ' ')
          << alloy::append('s', '!')
          << alloy::at(2)
          << data; // prints Say Hello Worlds!

    // with Alloy you can kiss `std::apply` goodbye
    print << alloy::unpack(std::make_tuple(3, '.', 1, 4)); // prints 3.14

    // `std::visit` is also a thing of the past
    constexpr std::variant<int, char, std::string_view> i = 3;
    constexpr std::variant<int, char, std::string_view> c = '.';
    constexpr std::variant<int, char, std::string_view> s = "1"sv;

    print << alloy::unpack(i, c, s); // prints 3.1

    // you can even mix tuples and variants together!
    print << alloy::unpack(i, c, s, std::make_tuple(4, "15")); // prints 3.1415

    // you can also provide custom data producers
    constexpr auto produce = [](auto&& consume) {
        return consume("Hello", ' ', "World");
    };

    // just don't forget to disambiguate the overloaded call to `operator <<`
    // the following are all equivalent and print Hello World
    print << alloy::source{produce};
    alloy::sink{print} << produce;
    alloy::sink{print} << alloy::source{produce};

    // likewise, you can also provide custom middle-ware
    constexpr auto process = [](auto&& consume) {
        return [&consume](auto&& hello, auto&& _, auto&& world) {
            return consume(hello, _, "brave", _, "new", _, world, '!');
        };
    };

    // it's enough to disambiguate a single entity in the pipeline
    print << alloy::stream{process} << produce; // prints Hello brave new World!

    // don't worry, disambiguators are just tags that come at zero-overhead

    print << alloy::forward("enjoy modern C++!");
}
/// [overview]
