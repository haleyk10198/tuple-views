//
// Created by kkwok9 on 1/8/24.
//

#ifndef TUPLE_VIEWS_TUPLE_RANGES_HXX
#define TUPLE_VIEWS_TUPLE_RANGES_HXX

#include <tuple>
#include <utility>
#include <array>
#include <iterator>
#include <algorithm>
#include <concepts>
#include <views/tuple_adaptor.hxx>

namespace ranges::tuple {
    struct find_if_fn : tuple_adaptor<find_if_fn> {
    private:
        template<typename Tup, typename Pred, std::size_t... Is> requires (
        std::predicate<Pred, std::tuple_element_t<Is, std::decay_t<Tup> > > && ...)

        constexpr std::size_t find_if_impl(Tup &&tup, Pred pred, std::index_sequence<Is...>) const {
            std::array arr{pred(std::get<Is>(std::forward<Tup>(tup)))...};
            return std::distance(std::ranges::begin(arr), std::ranges::find(arr, true));
        }

    public:
        template<typename Tup, typename Pred>
        constexpr std::size_t operator()(Tup &&tup, Pred pred) const {
            constexpr auto sz = std::tuple_size_v<std::decay_t<Tup>>;
            constexpr auto idx = std::make_index_sequence<sz>();

            return find_if_impl(std::forward<Tup>(tup), pred, idx);
        }

        static constexpr int arity_ = 2;

        using tuple_adaptor::operator();
    };

    constexpr auto find_if = find_if_fn{};

    struct find_fn : tuple_adaptor<find_fn> {
        template<typename Tup, typename T, std::size_t... Is>
        consteval std::size_t find_impl(Tup &&tup, T value, std::index_sequence<Is...>) const {
            constexpr auto pred = [tup_ = std::forward<Tup>(tup), value_ = std::forward<T>(value)]<std::size_t idx>() {
                auto field = std::get<idx>(std::forward<Tup>(tup_));
                if constexpr (std::equality_comparable_with<decltype(value_), decltype(field)>) {
                    return value_ == field;
                } else {
                    return false;
                }
            };
            constexpr std::array arr{pred.template operator()<Is>()...};
            return std::distance(std::ranges::begin(arr), std::ranges::find(arr, true));
        }

    public:
        template<typename Tup, typename T>
        consteval std::size_t operator()(Tup &&tup, T &&value) const {
            constexpr auto sz = std::tuple_size_v<std::decay_t<Tup>>;
            constexpr auto idx = std::make_index_sequence<sz>();

            return find_impl(std::forward<Tup>(tup), std::forward<T>(value), idx);
        }

        static constexpr int arity_ = 2;

        using tuple_adaptor::operator();

        static constexpr bool has_simple_extra_args_ = true;
    };

    constexpr auto find = find_fn{};
}

#endif //TUPLE_VIEWS_TUPLE_RANGES_HXX
