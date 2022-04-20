#include <iostream>
#include <string>
#include <vector>
#include <cassert>

using namespace std::literals;

enum DayOfWeek : uint8_t { mon = 1, tue, wed, thd, fri, sat, sun };

// scoped enumerations
enum class Months : uint8_t { jan = 1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec, unkown = 12 };

void calculate(int arg)
{
    assert(arg > 0); // dynamic assert
}

int main()
{
    static_assert(sizeof(DayOfWeek) == 1);

    // classic enum
    DayOfWeek day = mon;

    switch(day)
    {
    case mon:
        std::cout << "Poniedziałek" << std::endl;
            break;
    case tue:
        std::cout << "Wtorek" << std::endl;
        break;
    default:
        std::cout << "Inny dzien tygodnia\n";
    }

    int value = day;
    std::cout << "Value: " << value << "\n";

    Months month = Months::apr;

    value = static_cast<int>(month);
    assert(value == 4);

    month = Months::dec;

    Months another_month = Months::unkown;

    assert(month == another_month);
    std::cout << "month: " << static_cast<int>(month) << " - another_month: " << static_cast<int>(another_month) << "\n";

    uint8_t month_value = 8;

    month = static_cast<Months>(month_value);
    assert(month == Months::aug);
}
