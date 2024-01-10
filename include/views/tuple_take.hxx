//
// Created by kkwok9 on 1/8/24.
//

#ifndef TUPLE_VIEWS_TUPLE_TAKE_HXX
#define TUPLE_VIEWS_TUPLE_TAKE_HXX

#include <algorithms/tuple_find.hxx>
#include <tuple>
#include <utility>
#include "tuple_adaptor.hxx"
#include <utilities/tuple_traits.hxx>

namespace ranges::views::tuple {
    template<std::size_t sz>
    struct take_fn : tuple_adaptor_closure<take_fn<sz>> {
    private:
        template<typename Tup, std::size_t... Is>
        constexpr auto take_impl(Tup &&tup, std::index_sequence<Is...>) const {
            return std::make_tuple(std::get<Is>(tup)...);
        }

    public:
        template<typename Tup>
        constexpr auto operator()(Tup &&tup) const {
            constexpr auto idx = std::make_index_sequence<std::min(std::tuple_size_v<std::decay_t<Tup>>, sz)>();
            return take_impl(std::forward<Tup>(tup), idx);
        }
    };

    template<std::size_t sz>
    constexpr auto take = take_fn<sz>{};
}

#endif //TUPLE_VIEWS_TUPLE_TAKE_HXX
