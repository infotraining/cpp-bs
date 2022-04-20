#ifndef BANK_ACCOUNT_HPP
#define BANK_ACCOUNT_HPP

#include <string>

namespace Banking
{
    class BankAccount
    {
        int id_;
        std::string owner_;
        double balance_;
    public:
        // constructor
        BankAccount(int id, const std::string& owner,
            double balance);

        int id() const
        {
            return id_;
        }

        std::string owner() const
        {
            return owner_;
        }

        double balance() const
        {
            return balance_;
        }

        void deposit(double amount);

        void withdraw(double amount);
    };

    void print(const BankAccount& account);
}



#endif // BANK_ACCOUNT_HPP
