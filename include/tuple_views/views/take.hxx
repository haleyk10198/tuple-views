//
// Created by kkwok9 on 1/8/24.
//

#ifndef TUPLE_VIEWS_TAKE_HXX
#define TUPLE_VIEWS_TAKE_HXX

#include "tuple_views/algorithms/find.hxx"
#include <tuple>
#include <utility>
#include "tuple_adaptor.hxx"
#include "tuple_views/utilities/tuple_traits.hxx"

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

#define take_while(Tup, Pred) \
    __take_while_impl(Tup, Pred, __take_while_parameter_t_##__COUNTER__);

// NOLINTNEXTLINE
#define __take_while_impl(Tup, Pred, TT) \
    (take<(Tup) | find_if([]<typename TT>(TT&& field) -> bool { \
        return !std::invoke((Pred), std::forward<TT>(field));   \
    })>)(Tup)\

#endif //TUPLE_VIEWS_TAKE_HXX
