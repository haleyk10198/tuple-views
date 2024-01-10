// Created by kkwok9 on 1/8/24.
//

#ifndef TUPLE_VIEWS_TUPLE_DROP_HXX
#define TUPLE_VIEWS_TUPLE_DROP_HXX

#include <tuple>
#include <utility>
#include "tuple_adaptor.hxx"
#include <utilities/tuple_traits.hxx>

namespace ranges::views::tuple {
    template<std::size_t sz>
    struct drop_fn : tuple_adaptor_closure<drop_fn<sz>> {
    private:
        template<typename Tup, std::size_t... Is>
        constexpr auto drop_impl(Tup &&tup, std::index_sequence<Is...>) const {
            return std::make_tuple(std::get<Is + sz>(tup)...);
        }

    public:
        template<typename Tup>
        constexpr auto operator()(Tup &&tup) const {
            constexpr auto remaining_sz = ranges::tuple::_tuple_size_v<Tup> - sz;
            constexpr auto idx = std::make_index_sequence<remaining_sz>();
            return drop_impl(std::forward<Tup>(tup), idx);
        }
    };

    template<std::size_t sz>
    constexpr auto drop = drop_fn<sz>{};
}

#endif //TUPLE_VIEWS_TUPLE_DROP_HXX
