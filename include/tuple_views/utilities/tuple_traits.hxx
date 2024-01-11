//
// Created by kkwok9 on 1/10/24.
//

#ifndef TUPLE_VIEWS_TUPLE_TRAITS_HXX
#define TUPLE_VIEWS_TUPLE_TRAITS_HXX

#include <concepts>
#include <tuple>

namespace ranges::tuple {
    template<std::size_t I, typename Tup, typename Pred>
    concept _tuple_field_predicate = std::predicate<Pred const &, std::tuple_element_t<I, std::decay_t<Tup> > >;

    template<std::size_t I, typename Tup, typename Func>
    concept _tuple_field_invocable = std::invocable<Func const &, std::tuple_element_t<I, std::decay_t<Tup> > >;

    template<typename Tup>
    constexpr auto _tuple_size_v = std::tuple_size_v<std::decay_t<Tup>>;

    template<typename Tup>
    constexpr auto _tuple_index_sequence = std::make_index_sequence<_tuple_size_v<Tup>>();
}

#endif //TUPLE_VIEWS_TUPLE_TRAITS_HXX
