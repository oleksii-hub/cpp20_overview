#ifndef CPP_CONCEPTS_HPP
#define CPP_CONCEPTS_HPP

namespace concepts {
template<typename T>
concept IsPointer = requires (T p) {
    *p;
    p == nullptr;
    { p < p } -> std::convertible_to<bool>;
};

template<IsPointer T>
bool isLess(T a, T b) {
    return *a < *b;
}

bool isLess(auto a, auto b) {
    return a < b;
}

//-------------------------------------------------------------

template<auto Val>
concept MaxLimit = Val <= 1024;

template<typename T, std::size_t N>
requires MaxLimit<N>
struct Buffer {};

//-------------------------------------------------------------

int compareIntegrals(std::integral auto a, std::integral auto b)
requires std::totally_ordered_with<decltype(a), decltype(b)>
{
    if (a < b) {
        return -1;
    }

    if (a > b) {
        return 1;
    }

    return 0;
}

//-------------------------------------------------------------

template<typename T>
concept HasValueType = requires {
    typename T::value_type;
};

template<HasValueType T>
constexpr bool IsIntCollectionValue = std::is_same_v<typename T::value_type, int>;

//-------------------------------------------------------------

template<typename T, typename U>
concept MyIsSame = std::is_same_v<T, U>;

template<typename T, typename U>
requires MyIsSame<T, U>
void foo(T, U) {
}

template<typename T, typename U>
requires MyIsSame<T, U> && std::is_integral_v<T>
void foo(T, U) {
}

template<typename T, typename U>
requires MyIsSame<T, U>
void foo1(T, U) {
}

template<typename T, typename U>
requires MyIsSame<U, T> && std::is_integral_v<T>
void foo1(T, U) {
}

//-------------------------------------------------------------

template<typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

Arithmetic auto add(Arithmetic auto a, Arithmetic auto b) {
    return a + b;
}

template<Arithmetic T>
class Calculator {
    public:
    Calculator(T value) {

    }
};

//-------------------------------------------------------------

struct Mutex {
    void lock();
};

template<typename T> constexpr bool EnableThreadSafeGuarantee = false;
template<> constexpr bool EnableThreadSafeGuarantee<Mutex> = true;

template<typename T>
concept ThreadSafe = requires (T a)
{
    a.lock();
    requires EnableThreadSafeGuarantee<T> == true;
};

void mathOp(std::floating_point auto x) {
}

//-------------------------------------------------------------

// simple requirements
template<typename T>
concept SupportsMinusAndPlus = requires (T a, T b)
{
    a + b; // only assures this expression is valid
    a - b; // only assures this expression is valid
    std::integral<T>; // only assures this expression is valid
    requires std::integral<T>; // requires integral
};

// type requirements
template<typename  T>
concept ValidType = requires
{
    typename T::value_type;
    requires (std::integral<T> && !std::same_as<T, bool>);
    std::make_unsigned<T>::type; // OK, otherwise can be UB
};

// compound req
template<typename T>
concept CompoundConcept = requires (T a)
{
    { a == a } noexcept;
    { a == a } -> std::convertible_to<bool>;
};

//-------------------------------------------------------------

}

#endif // CPP_CONCEPTS_HPP