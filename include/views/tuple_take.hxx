//
// Created by kkwok9 on 1/8/24.
//

#ifndef TUPLE_VIEWS_TUPLE_TAKE_HXX
#define TUPLE_VIEWS_TUPLE_TAKE_HXX

#include <tuple>
#include <utility>
#include "tuple_adaptor.hxx"

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
        requires (std::tuple_size_v<std::decay_t<Tup>> >= sz)
        constexpr auto operator()(Tup &&tup) const {
            constexpr auto idx = std::make_index_sequence<sz>();
            return take_impl(std::forward<Tup>(tup), idx);
        }
    };

    template<std::size_t sz>
    constexpr auto take = take_fn<sz>{};

//    template<std::size_t sz>
//    struct take_while_fn : tuple_adaptor_closure<take_fn<sz>> {
//    private:
//        template<typename Tuple, typename Func, std::size_t ... N>
//        consteval auto tuple_find_if_impl(Tuple tup, Func func, std::index_sequence<N...>)
//        {
//            constexpr auto compares = std::array{func(std::get<N>(tup))...};
//            constexpr auto index = std::ranges::find(compares, true) - compares.begin();
//            return std::get<index>(tup);
//        }
//
//        template<typename Tup, std::size_t... Is>
//        constexpr auto take_while_impl(Tup &&tup, std::index_sequence<Is...>) const {
//            return std::make_tuple(std::get<Is>(tup)...);
//        }
//
//    public:
//        template<typename Tup, typename Pred>
//            requires (std::tuple_size_v<std::decay_t<Tup>> >= sz)
//        consteval auto operator()(Tup &&tup, Pred &&pred) const {
//            constexpr auto idx = std::make_index_sequence<sz>();
//            return take_impl(std::forward<Tup>(tup), idx);
//        }
//
//        static constexpr int arity_ = 2;
//        template<typename T>
//        static constexpr bool has_simple_extra_args = std::is_integral_v<T>;
//    };
//
//    template<std::size_t sz>
//    constexpr auto take_while = take_while_fn<sz>{};
}

#endif //TUPLE_VIEWS_TUPLE_TAKE_HXX
