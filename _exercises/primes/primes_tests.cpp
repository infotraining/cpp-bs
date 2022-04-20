#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include <iostream>
#include <string>
#include <cmath>

#include "catch.hpp"

using namespace std;

bool is_prime(const uint64_t n)
{
    if (n < 2)
        return false;

    for(uint64_t divisor = 2; divisor <= std::sqrt(n); ++divisor)
    {
        if (n % divisor == 0)
        {
            return false;
        }
    }

    return true;
}

std::vector<uint64_t> primes(const uint64_t limit)
{
    std::vector<uint64_t> found_primes;
    found_primes.reserve(100);

    for(uint64_t n = 2; n <= limit; ++n)
        if (is_prime(n))
            found_primes.push_back(n);

    return found_primes;
}

void print(const std::string& desc, const std::vector<uint64_t>& vec)
{
    std::cout << desc << ": ";

    for(const int& item : vec)
        std::cout << item << " ";
    std::cout << "\n";
}

TEST_CASE("is_prime", "[primes]")
{
    REQUIRE(is_prime(1) == false);
    REQUIRE(is_prime(13) == true);
    REQUIRE(is_prime(9) == false);

    REQUIRE_FALSE(is_prime(8));
    REQUIRE(is_prime(17));
}

TEST_CASE("primes", "[primes]")
{
    REQUIRE(primes(10) == vector<uint64_t>{ 2, 3, 5, 7 });

    using namespace Catch::Matchers;

    REQUIRE_THAT(primes(20), Equals(vector<uint64_t>{2, 3, 5, 7, 11, 13, 17, 19}));

    print("found primes", primes(100));
}

TEST_CASE("reference binding")
{
    std::vector<uint64_t> primes_to_100 = primes(100);

    SECTION("lvalue can be bound to lvalue ref")
    {
        std::vector<uint64_t>& ref_lvalue = primes_to_100;
    }

    SECTION("rvalue cannot be bound to lvalue ref")
    {
        //std::vector<uint64_t>& ref_lvalue = primes(100); // ERROR
    }

    SECTION("rvalue can be bound to lvalue ref to const")
    {
        const std::vector<uint64_t>& ref_lvalue = primes(100);
    }

    // since C++11
    SECTION("rvalue can be bound to rvalue ref")
    {
        std::vector<uint64_t>&& ref_rvalue = primes(100);
    }

    SECTION("lvalue cannot be bound to rvalue ref")
    {
        // std::vector<uint64_t>&& ref_rvalue = primes_to_100; // ERROR
    }

}
