//
// Created by kkwok9 on 1/8/24.
//

#ifndef TUPLE_VIEWS_TUPLE_VIEWS_H
#define TUPLE_VIEWS_TUPLE_VIEWS_H

#include "views/tuple_take.hxx"
#include "views/tuple_drop.hxx"
#include "views/tuple_reverse.hxx"
#include "views/tuple_zip.hxx"
#include "views/tuple_adaptor.hxx"
#include "views/tuple_adjacent.hxx"
#include "views/tuple_chunk.hxx"
#include "views/tuple_stride.hxx"
#include "views/tuple_as_const.hxx"

namespace views::tuple {
    using namespace ranges::views::tuple;
}

#endif //TUPLE_VIEWS_TUPLE_VIEWS_H
