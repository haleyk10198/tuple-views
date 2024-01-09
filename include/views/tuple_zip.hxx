//
// Created by kkwok9 on 1/9/24.
//

#ifndef TUPLE_VIEWS_TUPLE_ZIP_HXX
#define TUPLE_VIEWS_TUPLE_ZIP_HXX

#include <ranges>
#include <tuple>
#include <utility>

namespace ranges::views::tuple {
    struct zip_fn: tuple_adaptor<zip_fn>{
    private:
        template<typename Tup1, typename Tup2, std::size_t... Is>
        constexpr auto zip_impl(Tup1&& tup1, Tup2&& tup2, std::index_sequence<Is...>) const {
            return std::tuple { std::pair { std::get<Is>(std::forward<Tup1>(tup1)), std::get<Is>(std::forward<Tup2>(tup2)) }... };
        }
    public:
        template<typename Tup1, typename Tup2>
        constexpr auto operator()(Tup1&& tup1, Tup2&& tup2) const {
            constexpr auto sz1 = std::tuple_size_v<std::decay_t<Tup1>>;
            constexpr auto sz2 = std::tuple_size_v<std::decay_t<Tup2>>;
            constexpr auto sz_ = std::min(sz1, sz2);
            constexpr auto idx = std::make_index_sequence<sz_>();

            return zip_impl(std::forward<Tup1>(tup1), std::forward<Tup2>(tup2), idx);
        }

        static constexpr int arity_ = 2;
        template<typename>
        struct has_simple_extra_args_: std::false_type {};

        template<typename... Ts>
        struct has_simple_extra_args_<std::tuple<Ts...>>: std::true_type {};

        using tuple_adaptor<zip_fn>::operator();
    };

    constexpr auto zip = zip_fn{};
}

#endif //TUPLE_VIEWS_TUPLE_ZIP_HXX
