#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <list>

using namespace std::literals;

template <typename TContainer>
void print(const std::string& desc, const TContainer& vec)
{
    std::cout << desc << ": ";

    for(const auto& item : vec)
        std::cout << item << " ";
    std::cout << "\n";
}

struct Person
{
    std::string fname;
    std::string lname;
    int age;
};

std::ostream& operator<<(std::ostream& out, const Person& p)
{
    out << "Person{" << p.fname << " " << p.lname << ", age: " << p.age << "}";
    return out;
}

bool compare_by_age(const Person& p1, const Person& p2)
{
    return p1.age < p2.age;
}

struct AgeComparerDesc
{
    bool operator()(const Person& p1, const Person& p2)
    {
        return p1.age > p2.age;
    }
};

template <typename Iterator, typename Predicate>
Iterator find_when(Iterator first, Iterator last,
    Predicate predicate)
{
    for(auto it = first; it != last; ++it)
    {
        if (predicate(*it))
            return it;
    }

    return last;
}

bool is_older_than_18(const Person& p)
{
    return p.age > 18;
}

struct IsOlderThan
{
    int limit{};

    bool operator()(const Person& p) const
    {
        return p.age > limit;
    }
};

struct IsGreaterThen
{
    int limit{};

    bool operator()(int n) const
    {
        return n > limit;
    }
};

TEST_CASE("IsOlderThen")
{
    Person p{"Jan", "Kowalski", 30};

    IsOlderThan predicate{18};
    REQUIRE(predicate(p));
}

TEST_CASE("using functors")
{
    std::vector<Person> people = {
        Person{"Jan", "Nowak", 20},
        Person{"Ewa", "Kowalska", 33},
        Person{"Król", "Julian", 6}
        };

    std::sort(people.begin(), people.end(), &compare_by_age);
    print("people sorted by age", people);

    std::vector<Person>::iterator pos_gt_than_18 = std::find_if(people.begin(), people.end(), IsOlderThan{30});
    REQUIRE(pos_gt_than_18->lname == "Kowalska");

    std::sort(people.begin(), people.end(), AgeComparerDesc{});
    print("people sorted by age (descending)", people);


    std::list<int> numbers = { 5, 7, 1, 66, 665, 235, 645, 2345, 645, 66 };

    auto pos_gt_600 = std::find_if(numbers.begin(), numbers.end(), IsGreaterThen{600});
    REQUIRE(*pos_gt_600 == 665);

    SECTION("lambda expression")
    {
        auto pos_gt_600 = std::find_if(numbers.begin(), numbers.end(), [](int n) { return n > 600; });
        REQUIRE(*pos_gt_600 == 665);

        auto pos_jan = std::find_if(people.begin(), people.end(), [](const Person& p) { return p.fname == "Jan"; });

        auto adults_count = std::count_if(people.begin(), people.end(), [](const Person& p) { return p.age > 18; });
        REQUIRE(adults_count == 2);
    }
}


////////////////////////////////////


class Lambda_34523764528736
{
public:
    int operator()(int a, int b) const
    { return a + b; }
};

TEST_CASE("lambda explained")
{
    auto add = [](int a, int b) { return a + b; };

    SECTION("is interpreted as")
    {
        auto add = Lambda_34523764528736{};
    }

    REQUIRE(add(1, 2) == 3);


    SECTION("captures")
    {
        int age_limit = 18;

        SECTION("by value [=]")
        {
            auto is_gt_than_limit = [=](const Person& p) { return p.age > age_limit; };

            age_limit = 5;

            REQUIRE(!is_gt_than_limit(Person{"Gal", "Anonim", 15}));
        }

        SECTION("by reference [&]")
        {
            auto is_gt_than_limit = [&](const Person& p) { return p.age > age_limit; };

            age_limit = 5;

            REQUIRE(is_gt_than_limit(Person{"Gal", "Anonim", 15}));
        }
    }
}

