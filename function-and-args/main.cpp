#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

void foo(int x, double d) // passing by value primitive type - OK
{
    std::cout << "foo(" << x << ", " << d << ")\n";
    ++x;
}

namespace NotEfficient
{
    void print_vec(std::string name, std::vector<int> vec) // // passing by value large objects - not-efficient
    {
        std::cout << name << ": [ ";
        for(const int& item : vec)
            std::cout << item << " ";
        std::cout << "]\n";
    }
}

namespace Efficient
{
    void print_vec(const std::string& name, const std::vector<int>& vec)
    {
        std::cout << name << ": [ ";
        for(const int& item : vec)
            std::cout << item << " ";
        std::cout << "]\n";
    }
}

TEST_CASE("functions - passing by value - copy")
{
    double pi = 3.14;
    foo(42, pi); // 42 - rvalue, pi - lvalue

    std::vector<int> data = {1, 2, 3, 4}; // data - lvalue
    NotEfficient::print_vec("data"s, data);  // "data"s - rvalue, data - lvalue
    Efficient::print_vec("data"s, data);
}
