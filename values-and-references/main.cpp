#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

TEST_CASE("reference")
{
    int x = 10;

    int& ref_x = x; // reference to int
    REQUIRE(&x == &ref_x);

    ref_x = 42;
    REQUIRE(x == 42);

    std::vector<int> vec = {1, 2, 3, 4};

    std::vector<int>& ref_vec = vec;
    ref_vec[2] = 6;
    REQUIRE(vec == std::vector<int>{1, 2, 6, 4});

    SECTION("it works similar to const pointer")
    {
        std::vector<int>* const ptr_vec = &vec;
        (*ptr_vec)[2] = 3;
    }
}

TEST_CASE("reference to const")
{
    int x = 10;
    const int& cref_x = x;

    //cref_x += 2; // ERROR
    x += 2;

    REQUIRE(cref_x == 12);

    std::vector<int> vec = {1, 2, 3, 4};
    const std::vector<int>& cref_vec = vec;

    REQUIRE(cref_vec[0] == 1);
    REQUIRE(cref_vec.size() == 4);

    vec.push_back(5);
    REQUIRE(cref_vec.size() == 5);
}

TEST_CASE("const object & references")
{
    const int x = 665;
    //++x;

    const int& ref_x = x;

    auto& aref_x = x; // auto deduction -> const int&
}

TEST_CASE("iteration over container")
{
    std::vector<std::string> words = { "one", "two", "three" };

    for(std::string& w : words) // for(Read-Write)
    {
        w.append("!");
    }

    for(const std::string& w : words) // for(READ ONLY)
    {
        std::cout << w << " ";
    }
    std::cout << std::endl;

    SECTION("is interpreted as")
    {
        for(auto it = words.begin(); it != words.end(); ++it)
        {
            const std::string& w = *it;
            std::cout << w << " ";
        }
    }
}

TEST_CASE("regular types")
{
    int x = 42;
    int y = x; // copy
    REQUIRE(x == y);

    std::string txt1 = "abc";
    std::string txt2 = txt1; // copy
    REQUIRE(txt1 == txt2);

    std::vector<int> vec1 = {1, 2, 3};
    std::vector<int> vec2 = vec1; // copy
    REQUIRE(vec1 == vec2);
}
