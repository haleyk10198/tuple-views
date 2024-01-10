//
// Created by kkwok9 on 1/10/24.
//

#ifndef TUPLE_VIEWS_TUPLE_CHUNK_HXX
#define TUPLE_VIEWS_TUPLE_CHUNK_HXX

#include "tuple_adaptor.hxx"

namespace ranges::views::tuple {
    template<std::size_t sz> requires ( sz > 0 )
    struct chunk_fn : tuple_adaptor_closure<chunk_fn<sz>> {
    private:
        template<std::size_t base, typename Tup, std::size_t... Offsets>
        constexpr auto make_chunk(Tup &&tup, std::index_sequence<Offsets...>) const {
            return std::tuple{std::get<base + Offsets>(std::forward<Tup>(tup))...};
        }

        template<std::size_t base, typename Tup>
        constexpr auto make_chunk(Tup &&tup) const {
            constexpr auto remaining = std::min(sz, ranges::tuple::_tuple_size_v<Tup> - base);
            return make_chunk<base>(std::forward<Tup>(tup), std::make_index_sequence<remaining>());
        }

        template<typename Tup, std::size_t... Is>
        constexpr auto chunk_impl(Tup &&tup, std::index_sequence<Is...>) const {
            return std::tuple{make_chunk<Is * sz>(std::forward<Tup>(tup))...};
        }

    public:
        template<typename Tup>
        constexpr auto operator()(Tup &&tup) const {
            constexpr std::size_t chunks = (ranges::tuple::_tuple_size_v<Tup> + sz - 1) / sz;
            return chunk_impl(std::forward<Tup>(tup), std::make_index_sequence<chunks>());
        }
    };

    template<std::size_t sz>
    constexpr auto chunk = chunk_fn<sz>{};
}

#endif //TUPLE_VIEWS_TUPLE_CHUNK_HXX
