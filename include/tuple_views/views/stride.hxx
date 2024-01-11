//
// Created by kkwok9 on 1/10/24.
//

#ifndef TUPLE_VIEWS_STRIDE_HXX
#define TUPLE_VIEWS_STRIDE_HXX

#include "tuple_adaptor.hxx"
#include "tuple_views/utilities/tuple_traits.hxx"

namespace ranges::views::tuple {
    template<std::size_t sz>
    requires (sz > 0)
    struct stride_fn : tuple_adaptor_closure<stride_fn<sz>> {
    private:
        template<typename Tup, std::size_t... Is>
        constexpr auto stride_impl(Tup &&tup, std::index_sequence<Is...>) const {
            return std::tuple { std::get<Is * sz>(std::forward<Tup>(tup))... };
        }

    public:
        template<typename Tup>
        constexpr auto operator()(Tup &&tup) const {
            constexpr std::size_t return_sz = (ranges::tuple::_tuple_size_v<Tup> + sz - 1) / sz;
            return stride_impl(std::forward<Tup>(tup), std::make_index_sequence<return_sz>());
        }
    };

    template<std::size_t sz>
    constexpr auto stride = stride_fn<sz>{};
}

#endif //TUPLE_VIEWS_STRIDE_HXX
