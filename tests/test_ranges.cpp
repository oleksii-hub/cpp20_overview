#include <gtest/gtest.h>
#include <ranges>
#include <algorithm>

#include "ranges.hpp"

TEST(RangesTest, TestViews) {
    std::vector a {1, 2, 3, 4, 5};

    auto firstFour1 = std::views::take(a, 4);
    auto firstFour2 = std::views::take(std::move(a), 4);

    static_assert(std::is_same_v<decltype(firstFour1), std::ranges::take_view<std::ranges::ref_view<std::vector<int>>>>, "firstFour1 should be a take_view of a ref_view of vector<int>");
    static_assert(std::is_same_v<decltype(firstFour2), std::ranges::take_view<std::ranges::owning_view<std::vector<int>>>>, "firstFour2 should be a take_view of an owning_view of vector<int>");

    std::vector<int> expected {1, 2, 3, 4};
    EXPECT_EQ(std::vector<int>(firstFour2.begin(), firstFour2.end()), expected);
}

TEST(RangesTest, LazyEval) {
    namespace vws = std::views;

    std::vector a {1, 2, 3, 4, 5};

    std::size_t filterCount = 0;
    std::size_t transformCount = 0;
    auto someView = a
        | vws::filter([&filterCount](int x) { ++filterCount; return x % 2 == 0; })
        | vws::transform([&transformCount](int x) { ++transformCount; return x * 10; });

    int y = 0;
    for (const int x : someView) {
        y += x;
    }

    EXPECT_EQ(y, 20 + 40);
    EXPECT_EQ(filterCount, 5);
    EXPECT_EQ(transformCount, 2);

    filterCount = 0;
    transformCount = 0;
    y = 0;
    for (const int x : someView) {
        y += x;
    }

    EXPECT_EQ(y, 20 + 40);
    EXPECT_EQ(filterCount, 3); // begin is cached
    EXPECT_EQ(transformCount, 2);
}

TEST(RangesTest, LazyEval2) {
    namespace vws = std::views;

    std::vector a {1, 2, 3, 4, 5};

    std::size_t filterCount = 0;
    std::size_t transformCount = 0;
    auto someView = a
        | vws::transform([&transformCount](int x) { ++transformCount; return -x; })
        | vws::filter([&filterCount](int x) { ++filterCount; return x % 2 == 0; });

    int y = 0;
    for (const int x : someView) {
        y += x;
    }

    EXPECT_EQ(y, -2 - 4);
    EXPECT_EQ(filterCount, 5);
    EXPECT_EQ(transformCount, 7); // scan 1, 2, dereference 2, scan 3, 4, dereference 4, scan 5

    filterCount = 0;
    transformCount = 0;
    y = 0;
    for (const int x : someView) {
        y += x;
    }

    EXPECT_EQ(y, -2 - 4);
    EXPECT_EQ(filterCount, 3); // begin is cached
    EXPECT_EQ(transformCount, 5); // dereference 2, scan 3/4, dereference 4, scan 5
}

TEST(RangesTest, TestBasicRanges) {
    namespace rgs = std::ranges;
    namespace vws = rgs::views;

    std::vector a {3, 5, 1, 9, 4, 7, 2, 8, 6};
    rgs::sort(a);

    EXPECT_EQ(std::vector({1, 2, 3, 4, 5, 6, 7, 8, 9}), a);

    std::string b = "hello";
    rgs::sort(b);
    EXPECT_EQ(b, "ehllo");

    auto view1 = a | vws::take(3) | vws::drop(1);
    EXPECT_TRUE(rgs::equal(view1, std::vector({2, 3})));
    EXPECT_EQ(std::vector({2, 3}), std::vector(view1.begin(), view1.end()));
}