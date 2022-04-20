#ifndef BANK_ACCOUNT_HPP
#define BANK_ACCOUNT_HPP

#include <string>

namespace Banking
{
    class BankAccount
    {
        const int id_;
        std::string owner_;
        double balance_;
        static double interest_rate_; // declaration
    public:
        static void set_interest_rate(double rate)
        {
            interest_rate_ = rate;
        }

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

        void pay_interest(int days);
    };

    void print(const BankAccount& account);
}

#endif // BANK_ACCOUNT_HPP
