#include "bank_account.hpp"
#include <iostream>
#include <cassert>

using namespace std;
using namespace Banking;

// definition of interest_rate_
double BankAccount::interest_rate_ = 0.0;

Banking::BankAccount::BankAccount(int id, const string& owner, double balance)
    : id_{id}, owner_{owner}, balance_{balance}
{
}

void BankAccount::deposit(double amount)
{
    assert(amount > 0.0);
    balance_ += amount;
}

void BankAccount::withdraw(double amount)
{
    assert(amount > 0.0);
    balance_ -= amount;
}

void BankAccount::pay_interest(int days)
{
    double factor = days / 365.0;
    double interest = balance() * factor * interest_rate_;
    balance_ += interest;
}

namespace Banking
{
    void print(const BankAccount& account)
    {
        cout << "BankAccount(id: " << account.id()
                  << ", owner: " << account.owner()
                  << ", balance: " << account.balance() << ")\n";
    }
}
