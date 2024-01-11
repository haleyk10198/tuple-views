//
// Created by kkwok9 on 1/10/24.
//

#ifndef TUPLE_VIEWS_TUPLE_AS_CONST_HXX
#define TUPLE_VIEWS_TUPLE_AS_CONST_HXX

#include "tuple_adaptor.hxx"
#include <utilities/tuple_traits.hxx>

namespace ranges::views::tuple {
    struct as_const_fn: tuple_adaptor_closure<as_const_fn> {
    private:
        template<typename... Ts>
        constexpr auto make_as_const(Ts... args) const {
            return std::tuple<const Ts...> { args... };
        }

        template<typename Tup, std::size_t... Is>
        constexpr auto as_const_impl(Tup&& tup, std::index_sequence<Is...>) const {
           return make_as_const(std::get<Is>(std::forward<Tup>(tup))...);
        }
    public:
        template<typename Tup>
        constexpr auto operator()(Tup&& tup) const {
            return as_const_impl(std::forward<Tup>(tup), ranges::tuple::_tuple_index_sequence<Tup>);
        }
    };

    constexpr auto as_const = as_const_fn{};
}

#endif //TUPLE_VIEWS_TUPLE_AS_CONST_HXX
