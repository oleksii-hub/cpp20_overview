#include <gtest/gtest.h>
#include <type_traits>
#include "../src/spaceship_operator.hpp"

using namespace spaceship_operator;

TEST(SpaceshipTest, TestSimplePersonCompare) {
    static_assert(std::three_way_comparable<Person>, "Person should be three-way comparable");

    Person person1{"John", "Doe", 50000.0};
    Person person2{"Jane", "Smith", 60000.0};
    Person person3{"John", "Doe", 50000.0};
    Person person4{"John", "Doe", 50001.0};

    // person1 and person2 should not be equal
    EXPECT_FALSE(person1 == person2);
    EXPECT_TRUE(person1 != person2);

    // person1 and person3 should be equal
    EXPECT_TRUE(person1 == person3);
    EXPECT_FALSE(person1 != person3);

    // Compare using spaceship operator
    EXPECT_TRUE((person1 <=> person2) > 0);
    EXPECT_TRUE((person2 <=> person1) < 0);

    EXPECT_TRUE(person1 < person4);
    EXPECT_TRUE(person4 > person1);

    EXPECT_TRUE((person4 <=> person1) > 0);
}

TEST(SpaceshipTest, RectangleTest) {
    Rectangle r1{2.0, 3.0}; // Area = 6.0
    Rectangle r2{4.0, 5.0}; // Area = 20.0
    Rectangle r3{3.0, 2.0}; // Area = 6.0

    EXPECT_TRUE(r1 < r2);
    EXPECT_TRUE(r2 > r1);
    EXPECT_TRUE(r1 == r3);
    EXPECT_TRUE(r1 != r2);
    EXPECT_TRUE(r1 <= r2);
    EXPECT_TRUE(r2 >= r3);

    EXPECT_TRUE(r2 <=> r1 == std::partial_ordering::greater);
    EXPECT_TRUE(r1 <=> r3 == std::partial_ordering::equivalent);
}

TEST(SpaceshipTest, GeneralTest) {
    static_assert(std::is_same_v<decltype(1.0 <=> 1), std::partial_ordering>, "Comparison should yield partial_ordering");
    static_assert(!std::is_same_v<decltype(1.0 <=> 1), std::strong_ordering>, "Comparison should yield partial_ordering");

    static_assert(std::is_same_v<decltype(1 <=> 1), std::strong_ordering>, "Comparison should yield partial_ordering");

    static_assert(std::is_same_v<decltype(std::string {"abc"} <=> "abc"), std::strong_ordering>, "Comparison should yield partial_ordering");

    EXPECT_TRUE(1.0 <=> 1 == std::partial_ordering::equivalent);
    EXPECT_TRUE(1.1 <=> 1 == std::partial_ordering::greater);
    EXPECT_TRUE(1.0 <=> 2 == std::partial_ordering::less);
    EXPECT_TRUE(1 <=> 1 == std::partial_ordering::equivalent);
    EXPECT_TRUE(1 <=> 1 == std::strong_ordering::equal);

    auto cmp = std::weak_order(1, 2);
    static_assert(std::is_same_v<decltype(cmp), std::weak_ordering>, "Comparison should yield weak_ordering");
    EXPECT_TRUE(cmp == std::weak_ordering::less);

    int p = 3;
    int* a = nullptr;
    EXPECT_TRUE(p <=> p == 0);
    EXPECT_TRUE(&p <=> a != 0);

    double d = std::numeric_limits<double>::quiet_NaN();
    EXPECT_TRUE(d <=> d == std::partial_ordering::unordered);
}

TEST(SpaceshipTest, TestGenericTypeCompare) {
    static_assert(std::three_way_comparable<Value<double>>, "Value should be three-way comparable");

    if constexpr (std::three_way_comparable<Value<double>>) {
        EXPECT_TRUE(Value{50000.0} <=> Value{60000.0} == std::strong_ordering::less);
    }

    Value value1{50000.0};
    Value value2{60000.0};
    Value value3{50000.0};

    EXPECT_EQ(value1 <=> value2, std::strong_ordering::less);
    EXPECT_EQ(value1 <=> value3, std::strong_ordering::equivalent);
    EXPECT_EQ(value1 <=> value3, std::strong_ordering::equal);
    EXPECT_EQ(value1 <=> value2, std::strong_ordering::less);
    EXPECT_EQ(value1 <=> value3, std::strong_ordering::equivalent);
    EXPECT_EQ(value1 <=> value3, std::strong_ordering::equal);
    EXPECT_TRUE(value1 < value2);
    EXPECT_TRUE(value1 == value3);
}

TEST(SpaceshipTest, TestWrapperTypeCompare) {
    static_assert(std::three_way_comparable<Wrapper<int>>, "Wrapper should be three-way comparable");
    static_assert(std::three_way_comparable<Wrapper<double>>, "Wrapper should be three-way comparable");
    static_assert(std::three_way_comparable<Wrapper<std::vector<std::string>>>, "Wrapper should be three-way comparable");

    struct A {
        int a;
    };

    static_assert(!std::three_way_comparable<Wrapper<A>>, "Custom A is not three-way comparable");
}