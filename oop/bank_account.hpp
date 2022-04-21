#ifndef BANK_ACCOUNT_HPP
#define BANK_ACCOUNT_HPP

#include <string>
#include <vector>
#include <ostream>

namespace Banking
{
    enum class TransactionType : char
    {
        interest = 'I', withdraw = 'W', deposit = 'D'
    };

    struct Transaction
    {
        TransactionType type;
        double amount;
    };

    inline bool operator==(const Transaction& left, const Transaction& right)
    {
        return left.type == right.type && left.amount == right.amount;
    }

    inline bool operator!=(const Transaction& left, const Transaction& right)
    {
        return !(left == right);
    }

    inline std::ostream& operator<<(std::ostream& stream_out, const Transaction& t)
    {
        stream_out << "Transaction{type: " << static_cast<char>(t.type) << ", amount: " << t.amount << "}";
        return stream_out;
    }

    class BankAccount
    {
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

        const std::vector<Transaction>& transactions() const
        {
            return transactions_;
        }
    private:
        const int id_;
        std::string owner_;
        double balance_;
        std::vector<Transaction> transactions_;
        static double interest_rate_; // declaration
    };

    void print(const BankAccount& account);

    std::ostream& operator<<(std::ostream& out, const BankAccount& account);
}

#endif // BANK_ACCOUNT_HPP
