#include "bank_account.hpp"
#include <iostream>
#include <cassert>

using namespace std;
using namespace Banking;

Banking::BankAccount::BankAccount(int id, const string& owner, double balance)
{
    this->id_ = id;
    owner_ = owner;
    balance_ = balance;
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

namespace Banking
{
    void print(const BankAccount& account)
    {
        cout << "BankAccount(id: " << account.id()
                  << ", owner: " << account.owner()
                  << ", balance: " << account.balance() << ")\n";
    }
}
