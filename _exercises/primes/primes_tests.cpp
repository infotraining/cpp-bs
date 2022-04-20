#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include <iostream>
#include <string>
#include <cmath>

#include "catch.hpp"

using namespace std;

TEST_CASE("is_prime", "[primes]")
{
    REQUIRE(is_prime(13) == true);
    REQUIRE(is_prime(9) == false);

    REQUIRE_FALSE(is_prime(8));
    REQUIRE(is_prime(17));
}

TEST_CASE("primes", "[primes]")
{
    REQUIRE(primes(10) == vector<int>{ 2, 3, 5, 7 });

    using namespace Catch::Matchers;

    REQUIRE_THAT(primes(20), Equals(vector<int>{2, 3, 5, 7, 11, 13, 17, 19}));
}