//
// Created by kkwok9 on 1/9/24.
//

#ifndef TUPLE_VIEWS_TUPLE_FOR_EACH_HXX
#define TUPLE_VIEWS_TUPLE_FOR_EACH_HXX

#include <range/v3/utility/tuple_algorithm.hpp>
#include <views/tuple_adaptor.hxx>
#include <array>
#include <views/tuple_zip.hxx>

namespace ranges::tuple {
    struct for_each_fn : tuple_adaptor<for_each_fn> {
    private:
        template<typename Tup, typename Func, std::size_t... Is> requires (_tuple_field_invocable<Is, Tup, Func> && ...)

        constexpr auto for_each_impl(Tup &&tup, const Func &func, std::index_sequence<Is...>) const {
            (std::invoke(func, std::forward<std::tuple_element_t<Is, Tup>>(std::get<Is>(std::forward<Tup>(tup)))), ...);
        }

    public:
        template<typename Tup, typename Func>
        constexpr auto operator()(Tup &&tup, Func func) const {
            for_each_impl(std::forward<Tup>(tup), func, _tuple_index_sequence<Tup>);
        }

        static constexpr int arity_ = 2;

        using tuple_adaptor::operator();
    };

    constexpr auto for_each = for_each_fn{};

    struct enumerate_fn: tuple_adaptor_closure<enumerate_fn> {
    private:
        template<typename T, std::size_t... Is>
        constexpr auto array_to_tuple(T&& iota, std::index_sequence<Is...>) const {
            return std::tuple { iota[Is]... };
        }
    public:
        template<typename Tup>
        constexpr auto operator()(Tup&& tup) const {
            constexpr auto iota_ = std::views::iota(0) | std::views::take(_tuple_size_v<Tup>);
            return array_to_tuple(iota_, _tuple_index_sequence<Tup>) | views::tuple::zip(std::forward<Tup>(tup));
        }
    };

    constexpr auto enumerate = enumerate_fn{};

    struct for_each_indexed_fn : tuple_adaptor<for_each_indexed_fn> {
    public:
        template<typename Tup, typename Func>
        constexpr auto operator()(Tup &&tup, Func func) const {
            std::forward<Tup>(tup) | enumerate | for_each(std::move(func));
        }

        using tuple_adaptor::operator();

        static constexpr int arity_ = 2;
    };

    constexpr auto for_each_indexed = for_each_indexed_fn{};
}

#endif //TUPLE_VIEWS_TUPLE_FOR_EACH_HXX
