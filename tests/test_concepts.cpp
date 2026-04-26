 #include <gtest/gtest.h>
#include <list>
#include <queue>
#include <ranges>

#include "concepts.hpp"

using namespace concepts;

TEST(ConceptTest, TestBasicConcepts) {
    static_assert(IsPointer<int*>, "int* should satisfy IsPointer");
    static_assert(!IsPointer<int>, "int should not satisfy IsPointer");
    int a = 0;
    int b = 1;
    int* pA = &a;
    int* pB = &b;

    static_assert(IsPointer<decltype(pA)>, "pA should satisfy IsPointer");
    EXPECT_TRUE(isLess(a, b));
    EXPECT_TRUE(isLess(pA, pB));
    EXPECT_TRUE(isLess(&pA, &pB));

    static_assert(MaxLimit<512>, "512 should satisfy MaxLimit");
    static_assert(!MaxLimit<2048>, "2048 should not satisfy MaxLimit");

    Buffer<int, 512> buffer;
    // Buffer<int, 2048> bigBuffer; // should not compile
}

TEST(ConceptTest, compareAndPrintTest) {
    EXPECT_EQ(compareIntegrals(1, 2), -1);
    EXPECT_EQ(compareIntegrals(2, 1), 1);
    EXPECT_EQ(compareIntegrals(1, 1), 0);

    constexpr int a = 5;
    short b = 6;
    long c = 7;

    EXPECT_EQ(compareIntegrals(a, b), -1);
    EXPECT_EQ(compareIntegrals(a, c), -1);
    EXPECT_EQ(compareIntegrals(b, c), -1);
    EXPECT_EQ(compareIntegrals(c, a), 1);

    // Should not compile with non-integral types
    // compareIntegrals(1.0, 2.0);
    // compareIntegrals(1.f, 2.f);
}

TEST(ConceptTest, IsIntCollectionTest) {
    static_assert(IsIntCollectionValue<std::vector<int>>, "std::vector<int> should satisfy IsIntCollection");
    static_assert(!IsIntCollectionValue<std::vector<double>>, "std::vector<double> should not satisfy IsIntCollection");
    static_assert(!IsIntCollectionValue<std::vector<short>>, "std::vector<short> should not satisfy IsIntCollection");
    static_assert(!IsIntCollectionValue<std::vector<long>>, "std::vector<long> should not satisfy IsIntCollection");

    static_assert(IsIntCollectionValue<std::set<int>>, "std::set<int> should satisfy IsIntCollection");
    static_assert(IsIntCollectionValue<std::list<int>>, "std::list<int> should satisfy IsIntCollection");
    static_assert(IsIntCollectionValue<std::queue<int>>, "std::queue<int> should satisfy IsIntCollection");
}

int print(std::movable auto) {return 1;}
int print(std::copyable auto) {return 2;}

TEST(ConceptTest, Subsuming) {
    int a = 5, b = 5;
    foo(a, b);
    //foo1(a, b); // ambiguity error, order of the template params matter

    std::string s;
    EXPECT_EQ(print(s), 2); // copyable subsumes moveable
}

TEST(ConceptTest, SemanticConcept) {
    struct Mutex2 {
        void lock();
    };

    static_assert(ThreadSafe<Mutex>, "Mutex should satisfy ThreadSafe");
    static_assert(!ThreadSafe<Mutex2>, "Mutex should satisfy ThreadSafe");

    mathOp(7.f);
    mathOp(7.0);
    //mathOp(7); // error
}

SupportsMinusAndPlus auto func(SupportsMinusAndPlus auto value) {
    return value + value - value;
}

std::regular auto func2(std::regular auto value) {
    return value;
}

TEST(ConceptTest, SimpleConcept) {
    EXPECT_EQ(func(5), 5);
}

TEST(ConceptTest, CompoundTest) {
    struct typeA {
        bool operator==(const typeA &) const noexcept {
            return true;
        }
    };

    struct typeB {
        bool operator==(const typeA &) const {
            return true;
        }
    };

    static_assert(CompoundConcept<typeA>, "typeA should satisfy CompoundConcept");
    static_assert(!CompoundConcept<typeB>, "typeB should not satisfy CompoundConcept");
}

struct MySizedRange {
    int* begin() { return nullptr; }
    int* end() { return nullptr; }
};

struct MySizedRangeWithSize {
    int* begin() { return nullptr; }
    int* end() { return nullptr; }
    std::size_t size() { return 0; }
};

template<>
inline constexpr bool std::ranges::disable_sized_range<MySizedRange> = true;

TEST(ConceptsTest, OptOut) {
    static_assert(std::ranges::sized_range<MySizedRange>, "MySizedRange should satisfy sized_range");
    static_assert(std::ranges::sized_range<MySizedRangeWithSize>, "MySizedRange should satisfy sized_range");
}

