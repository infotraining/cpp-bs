#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "bank_account.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

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

TEST_CASE("BankAccount - interest rate")
{
    //BankAccount::set_interest_rate(0.1);

    BankAccount ba1{1, "Jan Kowalski", 100.0};

    ba1.set_interest_rate(0.1);

    ba1.pay_interest(365);

    REQUIRE(ba1.balance() == Approx(110.0));
}

TEST_CASE("BankAccount - transactions")
{
    BankAccount::set_interest_rate(0.1);
    BankAccount account {665, "Adam Nowak", 100.0};

    REQUIRE(account.transactions().size() == 0);

    account.pay_interest(365);
    account.withdraw(50.0);
    account.deposit(100.0);
    account.withdraw(1.0);

    std::vector<Transaction> expected_transactions = {
        Transaction {TransactionType::interest, 10.0},
        Transaction {TransactionType::withdraw, 50.0},
        Transaction {TransactionType::deposit, 100.0},
        Transaction {TransactionType::withdraw, 1.0}
    };

    REQUIRE(account.transactions() == expected_transactions);
}

TEST_CASE("operator== for transactions")
{
    int x = 4;
    int y = 4;
    REQUIRE(x == y);

    Transaction t1{TransactionType::interest, 1.0};
    Transaction t2{TransactionType::interest, 1.0};
    REQUIRE(t1 == t2);

    Transaction t3{TransactionType::deposit, 1.0};
    REQUIRE(t1 != t3);
}

TEST_CASE("operator << for transactions")
{
    Transaction t{TransactionType::deposit, 100.0};

    std::cout << t << "\n";
}


TEST_CASE("operator << for BankAccount")
{
    BankAccount account{665, "Jan Nowak", 100.0};

    std::stringstream ss;
    ss << account;

    REQUIRE(ss.str() == "BankAccount{id: 665, owner: Jan Nowak, balance: 100}");
}

TEST_CASE("withdraw too much")
{
    BankAccount account{666, "Lars", 1'000'000.0};

    REQUIRE_THROWS_AS(account.withdraw(2'000'000), InsufficientFundsError);
}
