#include "tuple_views.hxx"
#include "tuple_algorithms.hxx"
#include <string>
#include <gtest/gtest.h>

using namespace views::tuple;
using namespace ranges::tuple;

TEST(tuple_take, take_n_is_id) {
    std::tuple subject{1.0, 2, "3"};
    auto expected = subject;
    auto actual = subject | take<std::tuple_size_v<decltype(subject)>>;

    EXPECT_EQ(expected, actual);
}

TEST(tuple_take, take_lt_is_sunny) {
    std::tuple subject{1.0, 2, "3"};
    std::tuple expected{1, 2};
    auto actual = subject | take<2>;

    EXPECT_EQ(expected, actual);
}

TEST(tuple_take, take_empty) {
    std::tuple subject{1.0, 2, "3"};
    std::tuple expected{};
    auto actual = subject | take<0>;

    EXPECT_EQ(expected, actual);
}

TEST(tuple_drop, drop_empty_is_id) {
    std::tuple subject{1.0, 2, "3"};
    std::tuple expected = subject;
    auto actual = subject | drop<0>;

    EXPECT_EQ(expected, actual);
}

TEST(tuple_drop, drop_lt_is_trivial) {
    std::tuple subject{1.0, 2, "3"};
    std::tuple expected{"3"};
    auto actual = subject | drop<2>;

    EXPECT_EQ(expected, actual);
}

TEST(tuple_drop, drop_all_is_empty) {
    std::tuple subject{1.0, 2, "3"};
    std::tuple expected{};
    auto actual = subject | drop<std::tuple_size_v<decltype(subject)>>;

    EXPECT_EQ(expected, actual);
}

TEST(tuple_reverse, reverse_empty_is_id) {
    std::tuple subject{};
    std::tuple expected = subject;
    auto actual = subject | reverse;

    EXPECT_EQ(expected, actual);
}

TEST(tuple_reverse, reverse_is_trivial) {
    std::tuple subject{1.0, 2, "3"};
    std::tuple expected{"3", 2, 1.0};
    auto actual = subject | reverse;

    EXPECT_EQ(expected, actual);
}

TEST(tuple_find_if, find_homogeneous) {
    std::tuple subject{1, 2, 3};
    std::size_t expected = 1;
    auto foo = [](int ele) { return ele % 2 == 0; };
    auto actual = find_if(subject, foo);

    EXPECT_EQ(expected, actual);
}

TEST(tuple_find_if, find_adaptor) {
    std::tuple subject{1, 2, 3};
    std::size_t expected = 1;
    auto foo = [](int ele) { return ele % 2 == 0; };
    auto actual = subject | find_if(foo);

    EXPECT_EQ(expected, actual);
}

TEST(tuple_find_if, find_trait) {
    std::tuple subject{1.0, 2.0, "3", 4};
    constexpr std::size_t expected = 3;
    constexpr auto actual = find_if(subject, []<typename T>(T &&ele) -> bool {
        return std::is_integral_v<std::decay_t<T>>;
    });

    static_assert(expected == actual);
    EXPECT_EQ(expected, actual);
}

TEST(tuple_zip, zip_empty) {
    std::tuple fst{1.0, 2, "3"};
    std::tuple snd{};

    std::tuple expected = {};
    auto actual = zip(fst, snd);
    EXPECT_EQ(expected, actual);
}

TEST(tuple_zip, zip_adaptor_empty) {
    std::tuple fst{1.0, 2, "3"};
    std::tuple snd{};

    std::tuple expected = {};
    auto adaptor = zip(snd);

    auto actual = fst | adaptor;
    EXPECT_EQ(expected, actual);
}

TEST(tuple_zip, zip_adaptor_same_size) {
    std::tuple fst{1.0, 2, "3"};
    std::tuple snd{"one", "two", "three"};

    std::tuple expected = {std::pair{1.0, "one"}, std::pair{2, "two"}, std::pair{"3", "three"}};
    auto adaptor = zip(snd);

    auto actual = fst | adaptor;
    EXPECT_EQ(expected, actual);
}

TEST(tuple_zip, zip_adaptor_shrink_size) {
    std::tuple fst{1.0, 2, "3"};
    std::tuple snd{"one"};

    std::tuple expected = {std::pair{1.0, "one"}};
    auto adaptor = zip(snd);

    auto actual = fst | adaptor;
    EXPECT_EQ(expected, actual);
}

TEST(tuple_zip, zip_adaptor_tuck_size) {
    std::tuple fst{1.0};
    std::tuple snd{"one", "two", "three"};

    std::tuple expected = {std::pair{1.0, "one"}};
    auto adaptor = zip(snd);

    auto actual = fst | adaptor;
    EXPECT_EQ(expected, actual);
}

TEST(tuple_find, find_found) {
    constexpr std::tuple fst{"1", 2, "3"};
    std::size_t expected = 1;
    constexpr auto adaptor = find(2);

    auto actual = find(fst, 2);
    EXPECT_EQ(expected, actual);
}