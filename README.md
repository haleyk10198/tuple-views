# tuple-views

Some boiler-plates similar to std::ranges API to interact with tuple as if they are views,
packaged under `views::tuple` namespace not to be confused with actual ranges

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
- How often is this vararg function on the hot path instead of being some utility function?

## Caveats

- Tuples are not homogeneous hence not supported by std::ranges
- Return types are not guaranteed known at compile-type, so calls like filter are `consteval`
- Things like `find` are also `consteval` because they expect users to `std::get` with it and consumes an constexpr
- No adaptor syntax / `operator|` support for `consteval` algorithms due to limitation of C++ language model.
- Supports std::tuple only, sorry, no `boost::tuple`