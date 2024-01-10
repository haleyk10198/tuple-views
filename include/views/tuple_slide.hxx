//
// Created by kkwok9 on 1/10/24.
//

#ifndef TUPLE_VIEWS_TUPLE_SLIDE_HXX
#define TUPLE_VIEWS_TUPLE_SLIDE_HXX

#include "tuple_adaptor.hxx"
#include <utilities/tuple_traits.hxx>

namespace ranges::views::tuple {
    template<std::size_t sz>
    struct slide_fn : tuple_adaptor_closure<slide_fn<sz>> {
    private:
        static constexpr auto offsets = std::make_index_sequence<sz>();

        template<typename Tup, std::size_t I, std::size_t... Offsets>
        constexpr auto make_chunk(Tup &&tup, std::index_sequence<Offsets...>) const {
            return std::tuple{std::get<I + Offsets>(std::forward<Tup>(tup))...};
        }

        template<typename Tup, std::size_t... Is>
        constexpr auto slide_impl(Tup &&tup, std::index_sequence<Is...>) const {
            return std::tuple{make_chunk<Is>(std::forward<Tup>(tup), offsets) ...};
        }

    public:
        template<typename Tup>
        constexpr auto operator()(Tup &&tup) const {
            if constexpr (ranges::tuple::_tuple_size_v<Tup> > 0 && ranges::tuple::_tuple_size_v<Tup> < sz) {
                return slide_impl(std::forward<Tup>(tup),
                                  std::make_index_sequence<ranges::tuple::_tuple_size_v<Tup> + 1 - sz>());
            } else {
                return std::tuple{};
            }
        }
    };

    template<std::size_t sz>
    constexpr auto slide = slide_fn<sz>{};
}

#endif //TUPLE_VIEWS_TUPLE_SLIDE_HXX
