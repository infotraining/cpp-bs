#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

template <typename TContainer>
void print(const std::string& desc, const TContainer& vec)
{
    std::cout << desc << ": ";

    for(const auto& item : vec)
        std::cout << item << " ";
    std::cout << "\n";
}

class Integer
{
    int value_;
public:
    explicit Integer(int value) // may be used to implicit conversion from int -> Integer
        : value_{value}
    {}

    int value() const
    {
        return value_;
    }

    // option 1
    Integer operator-() const
    {
        return Integer{-value_};
    }

    // pre-incrementation
    Integer& operator++()
    {
        ++value_;
        return *this;
    }

    // post-incrementation
    const Integer operator++(int)
    {
        Integer prev = *this;
        ++value_;
        return prev;
    }

    // operator of conversion to int
    operator int() const
    {
        return value_;
    }
};

bool operator==(const Integer& left, const Integer& right)
{
    return left.value() == right.value();
}

bool operator!=(const Integer& left, const Integer& right)
{
    return !(left == right);
}

bool operator<(const Integer& left, const Integer& right)
{
    return left.value() < right.value();
}

Integer operator+(const Integer& left, const Integer& right)
{
    return Integer{left.value() + right.value()};
}

Integer operator-(const Integer& left, const Integer& right)
{
    return Integer{left.value() - right.value()};
}

// alternative implementation
//Integer operator-(const Integer& i)
//{
//    return Integer{-i.value()};
//}


std::ostream& operator<<(std::ostream& out, const Integer& i)
{
    out << "Integer{" << i.value() << "}";
    return out;
}

TEST_CASE("operators")
{
    Integer i1(10);
    Integer i2(20);

    SECTION("operator ==")
    {
        REQUIRE(i1 == Integer(10));

        REQUIRE(i1 == 10); // implicit conversion from 10 -> Integer(10)
        REQUIRE(10 == i1); // implicit conversion from 10 -> Integer(10)
    }

    SECTION("operator <")
    {
        std::vector<Integer> vec = { Integer(10), Integer(1), Integer(11), Integer(2), Integer(6) };

        std::sort(vec.begin(), vec.end());

        print("vec", vec);
    }

    SECTION("operator +")
    {
        REQUIRE(i1 + i2 == Integer(30));
        REQUIRE(i1 + 100 == 110);
    }

    SECTION("operator -")
    {
        SECTION("binary - ")
        {
            REQUIRE(i2 - i1 == 10);
        }

        SECTION("unary - ")
        {
            REQUIRE(-i1 == -10);
        }
    }

    SECTION("++")
    {
        SECTION("pre-incrementation")
        {
            Integer i = ++i1;
            REQUIRE(i == 11);
            REQUIRE(i1 == 11);

            int x = 10;
            ++++++++++++x;
            REQUIRE(x == 16);

            Integer ix(10);
            ++++++++++++ix;
            REQUIRE(ix == 16);
        }

        SECTION("post-incrementation")
        {
            Integer i = i1++;
            REQUIRE(i == 10);
            REQUIRE(i1 == 11);

            int x = 10;
            //x++++++;

            Integer ix(10);
            //ix++++++;
        }
    }

    SECTION("operator of conversion")
    {
        int x = Integer(10);
        REQUIRE(x == 10);
    }
}

void foo()
{
    std::cout << "foo()\n";
}

struct Foo
{
    size_t count{};

    void operator()()
    {
        std::cout << "Foo::operator()\n";
        ++count;
    }
};


TEST_CASE("operator ()")
{
    foo();

    Foo bar;

    bar();
    bar();
    bar();

    REQUIRE(bar.count == 3);

    Foo bar_other;
    REQUIRE(bar_other.count == 0);

    bar_other();
    bar_other();

    REQUIRE(bar_other.count == 2);
}
