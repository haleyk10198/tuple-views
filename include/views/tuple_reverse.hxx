//
// Created by kkwok9 on 1/8/24.
//

#ifndef TUPLE_VIEWS_TUPLE_REVERSE_HXX
#define TUPLE_VIEWS_TUPLE_REVERSE_HXX

#include <tuple>
#include <utility>
#include "tuple_adaptor.hxx"
#include <utilities/tuple_traits.hxx>

namespace ranges::views::tuple {
    struct reverse_fn: tuple_adaptor_closure<reverse_fn> {
    private:
        template<typename Tup, std::size_t... Is>
        constexpr auto reverse_impl(Tup &&tup, std::index_sequence<Is...>) const {
            return std::make_tuple(std::get<std::tuple_size_v<std::decay_t<Tup>> - 1 - Is>(tup)...);
        }

    public:
        template<typename Tup>
        constexpr auto operator()(Tup &&tup) const {
            return reverse_impl(std::forward<Tup>(tup), ranges::tuple::_tuple_index_sequence<Tup>);
        }
    };

    constexpr auto reverse = reverse_fn{};
}

#endif //TUPLE_VIEWS_TUPLE_REVERSE_HXX
