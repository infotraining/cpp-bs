#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>

using namespace std::literals;

void print_cstyle(const int* tab, size_t size)
{
    const int* const end = tab + size;
    for(const int* ptr = tab; ptr != end; ++ptr)
    {
        std::cout << *ptr << " ";
    }
    std::cout << std::endl;
}


template <typename Iterator>
void print_cppstyle(Iterator first, Iterator last)
{
    for(Iterator ptr = first; ptr != last; ++ptr)
    {
        std::cout << *ptr << " ";
    }
    std::cout << std::endl;
}

//void print_cppstyle(std::vector<int>::iterator first, std::vector<int>::iterator last)
//{
//    for(auto ptr = first; ptr != last; ++ptr)
//    {
//        std::cout << *ptr << " ";
//    }
//}


TEST_CASE("c-style array")
{
    const int N = 10;
    int tab[N] = {1, 2, 3, 4};

    REQUIRE(tab[2] == 3);

    SECTION("c-style iteration with pointer - read-only")
    {
        const int* const end = tab + 10;
        for(const int* ptr = tab; ptr != end; ++ptr)
        {
            std::cout << *ptr << " ";
        }
        std::cout << std::endl;
    }

    SECTION("c-style iteration with pointer - read-write")
    {
        int* const end = tab + 10;
        for(int* ptr = tab; ptr != end; ++ptr)
        {
            *ptr += 2;
        }

        print_cstyle(tab, N);
    }

    SECTION("range-based for")
    {
        for(const int& item : tab)
        {
            std::cout << item << " ";
        }
        std::cout << std::endl;

        SECTION("is iterpreted")
        {
            for(auto it = std::begin(tab); it != std::end(tab); ++it)
            {
                const int item = *it;
                std::cout << item << " ";
            }
            std::cout << std::endl;
        }
    }
}

TEST_CASE("C++ array")
{
    const int N = 10;
    std::array<int, N> tab = {1, 2, 3, 4};

    REQUIRE(tab[2] == 3);
    REQUIRE(tab.size() == 10);

    tab.fill(6);
    print_cstyle(tab.data(), tab.size());
    print_cppstyle(tab.begin(), tab.end());

    SECTION("range based for")
    {
        for(const int item : tab)
        {
            std::cout << item << " ";
        }
        std::cout << std::endl;

        SECTION("is iterpreted")
        {
            for(auto it = tab.begin(); it != tab.end(); ++it)
            {
                const int item = *it;
                std::cout << item << " ";
            }
            std::cout << std::endl;
        }
    }
}

TEST_CASE("dynamic size - c-style")
{
    std::cout << "dynamic array:" << std::endl;
    size_t  size = 100;
    int* tab = static_cast<int*>(malloc(size * sizeof(int)));

    for(int i = 0; i < 100; ++i)
        tab[i] = i;

    print_cstyle(tab, size);

    free(tab);
}

bool is_zero(int x)
{
    return x == 0;
}

TEST_CASE("dynamic array - C++")
{
    std::cout << "vector:" << std::endl;

    size_t  size = 100;

    std::vector<int> vec(size);

    REQUIRE(vec.size() == 100);

    REQUIRE(std::all_of(vec.begin(), vec.end(), is_zero));

    for(size_t i = 0u; i < vec.size(); ++i)
    {
        vec[i] = i;
    }

    vec.push_back(100);

    REQUIRE(vec.size() == 101);
    REQUIRE(vec.capacity() >= vec.size());


    std::vector<int>::iterator first = vec.begin();
    std::vector<int>::iterator last = vec.end();
    print_cppstyle(first, last);

    std::cout << "Sum: " << std::accumulate(vec.begin(), vec.end(), 0) << "\n";
}

TEST_CASE("vector - API")
{
    SECTION("default construction")
    {
        std::vector<int> vec_empty;

        REQUIRE(vec_empty.size() == 0);
        REQUIRE(vec_empty.capacity() == 0);
    }

    SECTION("constructor with size")
    {
        std::vector<std::string> words(4); // ["", "", "", ""]
        REQUIRE(words.size() == 4);

        std::vector<std::string> names(4, "Jan"); // ["Jan", "Jan", "Jan", "Jan"]
    }

    SECTION("constructor with list - since C++11")
    {
        std::vector<int> vec = {1, 2, 3, 4};
        REQUIRE(vec.size() == 4);

        print_cppstyle(vec.begin(), vec.end());
    }

    SECTION("beware with {}")
    {
        std::vector<int> vec_a(3, 4); // [4, 4, 4]

        std::vector<int> vec_b{3, 4}; // [3, 4]
    }

    SECTION("loops")
    {
        std::vector<int> vec = {1, 2, 3, 4};

        SECTION("with iterators")
        {
            for(std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
            {
                *it += 2;
            }


            for(std::vector<int>::const_iterator it = vec.begin(); it != vec.end(); ++it)
            {
                std::cout << *it << " ";
            }
            std::cout << std::endl;
        }
    }

    SECTION("push_backs")
    {
        std::vector<int> vec;
        vec.reserve(100);

        REQUIRE(vec.capacity() == 100);

        for(int i = 0; i < 100; i += 2)
            vec.push_back(i);

        REQUIRE(vec.size() == 50);
        REQUIRE(vec.capacity() == 100);

        print_cppstyle(vec.begin(), vec.end());
    }

    SECTION("resize")
    {
        std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        REQUIRE(vec.size() == 10);

        vec.resize(20);

        print_cppstyle(vec.begin(), vec.end());

        vec.resize(5);

        print_cppstyle(vec.begin(), vec.end());

        //vec.resize(0);
        vec.clear();
        REQUIRE(vec.size() == 0);
        REQUIRE(vec.capacity() >= 20);

        vec.shrink_to_fit();
        REQUIRE(vec.size() == 0);
        REQUIRE(vec.capacity() == 0);
    }

    SECTION("insert & erase")
    {
        std::vector<int> vec = { 5, 3, 6, 6, 8, 10, 1, 2, 10, 10, 3, 10 };

        std::vector<int>::iterator pos = vec.begin();
        pos += 4;
        vec.insert(pos, -1);

        print_cppstyle(vec.begin(), vec.end());

        pos = vec.begin();
        while(pos != vec.end())
        {
            pos = std::find(vec.begin(), vec.end(), 10);

            if (pos != vec.end()) // check if 10 is found
                pos = vec.erase(pos); // erase can invalidate iterator
        }

        REQUIRE(std::find(vec.begin(), vec.end(), 10) == vec.end());

        print_cppstyle(vec.begin(), vec.end());

        std::sort(vec.begin(), vec.end());

        print_cppstyle(vec.begin(), vec.end());
    }
}

