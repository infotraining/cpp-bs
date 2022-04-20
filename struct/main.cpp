#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std::literals;

struct Data
{
    int id;
    double value;
    const char* txt;
};


TEST_CASE("struct")
{
    Data d1; // uninitialized struct

    Data d2 = {1, 3.14, "pi"};
    REQUIRE(d2.id == 1);
    REQUIRE(d2.value == Approx(3.14));

    Data d3 = {};
    REQUIRE(d3.id == 0);
    REQUIRE(d3.value == Approx(0.0));
    REQUIRE(d3.txt == nullptr);

    Data d4 = d2;

    REQUIRE(d4.id == 1);
    REQUIRE(d4.value == Approx(3.14));

    std::cout << std::chrono::system_clock::now() << std::endl;
}
