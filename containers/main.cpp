#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>

using namespace std::literals;

template <typename TContainer>
void print(const std::string& desc, const TContainer& vec)
{
    std::cout << desc << ": ";

    for(const auto& item : vec)
        std::cout << item << " ";
    std::cout << "\n";
}

TEST_CASE("std::vector")
{
    std::vector<int> vec = {1, 2, 3, 4, 5};

    vec.push_back(6);
    vec.push_back(7);

    std::vector<int>::iterator pos = std::find(vec.begin(), vec.end(), 4);
    vec.insert(pos, -1);

    print("vec", vec);

    std::sort(vec.begin(), vec.end());

    print("vec sorted", vec);

    SECTION("vector has random_access_iterator")
    {
        std::vector<int>::iterator it = vec.begin();
        it += 4;
        REQUIRE(*it == 4);
    }
}

TEST_CASE("std::list")
{
    std::list<int> lst = {1, 2, 3, 4, 5};

    lst.push_back(6);
    lst.push_back(7);
    lst.push_front(0);

    std::list<int>::iterator pos = std::find(lst.begin(), lst.end(), 4);
    lst.insert(pos, -1);

    print("lst", lst);

    lst.sort();

    SECTION("list has bidirectional_iterator")
    {
        std::list<int>::iterator it = lst.begin();
//        ++it;
//        ++it;
//        ++it;
//        ++it;
        std::advance(it, 4);
        REQUIRE(*it == 3);
    }
}

TEST_CASE("map")
{
    SECTION("std::pair")
    {
        std::pair<int, std::string> p1{1, "one"};
        REQUIRE(p1.first == 1);
        REQUIRE(p1.second == "one");
        auto p2 = std::make_pair(2, "two"s);
    }

    std::unordered_map<int, std::string> dict = { {1, "one"}, {2, "two"}, {3, "three"}, {5, "five"} };

    REQUIRE(dict[2] == "two");

    dict.insert(std::make_pair(4, "four"));
    dict[6] = "six";
    dict.erase(3);

    // beware - indexing [] has side effect
    //dict[7];

    //std::map<int, std::string>::iterator pos = dict.find(7);
    auto pos = dict.find(7);
    if (pos == dict.end())
    {
        std::cout << "7 is not in dictionary\n";
    }

    for(const auto& item : dict)
    {
        std::cout << item.first << ":" << item.second << " ";
    }
    std::cout << "\n";
}
