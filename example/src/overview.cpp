// NOTE: keep in sync with the README

/// [overview]
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
/// [overview]
