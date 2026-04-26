#ifndef CPP_MODERN_SPACESHIP_OPERATOR_H
#define CPP_MODERN_SPACESHIP_OPERATOR_H

#include <string>

namespace spaceship_operator {

class Person {
    std::string name;
    std::string surname;
    double salary;

public:
    Person(std::string name, std::string surname, double salary)
        : name(std::move(name)), surname(std::move(surname)), salary(salary)
    {}

    auto operator<=>(const Person & other) const = default;
};

class Rectangle {
    double width;
    double height;

public:
    Rectangle(double width, double height)
        : width(width), height(height) {}

    bool operator==(const Rectangle & other) const noexcept {
        return width * height == other.width * other.height;
    }

    std::partial_ordering operator<=>(const Rectangle& other) const noexcept {
        return width * height <=> other.width * other.height;
    }
};

template <typename T>
class Value {
    T m_value;

public:
    Value(T value)
        : m_value(value)
    {}

    std::compare_three_way_result_t<T,T>
    operator<=>(const Value& other) const noexcept (noexcept(m_value <=> other.m_value)) = default;
};

template <typename T>
class Wrapper {
    T m_value;

public:
    Wrapper(T value)
        : m_value(value)
    {}

    bool operator==(const Wrapper& other) const noexcept requires(std::equality_comparable<T>)
    {
        return *this <=> other == 0;
    }

    auto operator<=>(const Wrapper& other) const noexcept requires(std::three_way_comparable<T>)
    {
        return m_value <=> other.m_value;
    }
};


}

#endif //CPP_SPACESHIP_OPERATOR_H