//
// Created by kkwok9 on 1/9/24.
//

#ifndef TUPLE_VIEWS_TUPLE_FOR_EACH_INDEXED_HXX
#define TUPLE_VIEWS_TUPLE_FOR_EACH_INDEXED_HXX

#include <range/v3/utility/tuple_algorithm.hpp>

namespace ranges::tuple {
    using for_each_fn = ranges::tuple_for_each_fn;

    constexpr auto for_each = for_each_fn{};
}

#endif //TUPLE_VIEWS_TUPLE_FOR_EACH_INDEXED_HXX
