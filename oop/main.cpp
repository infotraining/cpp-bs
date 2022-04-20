#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "bank_account.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;
using namespace Banking;

struct Date
{
    int day, month, year;

    std::string to_string() const
    {
        return std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
    }
};

TEST_CASE("Date - struct with method")
{
    Date today{20, 4, 2022};

    REQUIRE(today.to_string() == "20/4/2022");

    Date invalid_date{20};
}


TEST_CASE("BankAccount - construction")
{
    BankAccount ba1{1, "Jan Kowalski", 100.0};

    REQUIRE(ba1.id() == 1);
    REQUIRE(ba1.owner() == "Jan Kowalski");
    REQUIRE(ba1.balance() == Approx(100.0));
}

TEST_CASE("BankAccount - deposit")
{
    BankAccount ba1{1, "Jan Kowalski", 100.0};

    ba1.deposit(50.0);

    REQUIRE(ba1.balance() == Approx(150.0));
}

TEST_CASE("BankAccount - withdraw")
{
    BankAccount ba1{1, "Jan Kowalski", 100.0};

    ba1.withdraw(50.0);

    REQUIRE(ba1.balance() == Approx(50.0));

    Banking::print(ba1);
}
