# tuple-views

Some boiler-plates similar to std::ranges API to interact with tuple as if they are views,
packaged under `views::tuple` namespace not to be confused with actual ranges

## Installation

This library is headers only, just clone to include path and use by `#include <tuple_views/all.hxx>`.

Requires C++20 for concept checks. If this idea matures we can do C++14 SFINAEs.

## Motivation

Main motivation was to work with variadic templates easier.

```c++
template<typename Func, typename... Arg>
auto i_hate_parameter_pack(Func&& func, Arg... args) {
    std::tuple tuple { args... };
    ranges::tuple_apply(func, aggs | views::reverse); // tuple_apply from range-v3
}
```

- Do you write vararg & parameter pack syntax often enough to remember how to reverse pack?
- How often is this vararg function on the hot path instead of being some utility function that you want to trade readability for performance?

## Caveats

- There are no support for filter / take_while etc. C++ requires typing in compile time. Consider

```c++
std::tuple foo {1, "2"};
std::tuple tup1 = foo | take_while([](auto&& x) { /* ... is integer ... */ }) // {1}
std::tuple tup2 = foo | take_while({[](auto &&x) { /* ... is string... (/} }) // {2}

template<typename Tup, typename Pred> 
constexpr auto take_while(Tup&& tup, Pred&& pred) { // same call-signature for {1}, {2}
    return ...; // return type for {1}, {2} differs
}
```

A workaround is to write 

```c++
take<foo | find_if([](auto&& x) { return !/* ... your pred, NEGATED! ... */; } )(foo) >
take_while(foo, [](auto&& x) { return /* ... your pred ... */; }); // calls macro
```

The `take_while` macro is far from perfect though, for example, it doesn't support adaptor syntax.

## Comparison to range-v3

- More calls than what's in `<range/v3/utilitye/tuple_algorithm.hpp>`
- Adaptor syntax support
