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
#include "tuple_views/utilities/tuple_traits.hxx"
#include "tuple_views/views/tuple_adaptor.hxx"

namespace ranges::tuple {
    struct find_if_fn : tuple_adaptor<find_if_fn> {
    private:
        template<typename Tup, typename Pred, std::size_t... Is> requires (_tuple_field_predicate<Is, Tup, Pred> && ...)

        constexpr std::size_t find_if_impl(Tup &&tup, Pred const &pred, std::index_sequence<Is...>) const {
            std::array arr{pred(std::get<Is>(std::forward<Tup>(tup)))...};
            return std::distance(std::ranges::begin(arr), std::ranges::find(arr, true));
        }

    public:
        template<typename Tup, typename Pred>
        constexpr std::size_t operator()(Tup &&tup, Pred pred) const {
            constexpr auto sz = _tuple_size_v<Tup>;

            return find_if_impl(std::forward<Tup>(tup), pred, _tuple_index_sequence<Tup>);
        }

        static constexpr int arity_ = 2;

        using tuple_adaptor::operator();
    };

    constexpr auto find_if = find_if_fn{};

    struct find_fn: tuple_adaptor<find_fn> {
        template<typename F, typename T>
        constexpr bool cmp_field(const F &field, const T &value) const {
            if constexpr (std::equality_comparable_with<F, T>) {
                return field == value;
            } else {
                return false;
            }
        }

        template<typename Tup, typename T, std::size_t... Is>
        constexpr std::size_t find_impl(Tup tup, T value, std::index_sequence<Is...>) const {
            std::array arr{cmp_field(std::get<Is>(tup), value) ...};
            return std::distance(std::ranges::begin(arr), std::ranges::find(arr, true));
        }

    public:
        template<typename Tup, typename T>
        constexpr std::size_t operator()(Tup tup, T value) const {
            constexpr auto sz = std::tuple_size_v<std::decay_t<Tup>>;

            return find_impl(tup, value, _tuple_index_sequence<Tup>);
        }

        static constexpr int arity_ = 2;

        using tuple_adaptor::operator();

        static constexpr bool has_simple_extra_args_ = true;
    };

    constexpr auto find = find_fn{};
}

#endif //TUPLE_VIEWS_TUPLE_RANGES_HXX
