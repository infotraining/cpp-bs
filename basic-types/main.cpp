#include <iostream>
#include <string>
#include <vector>
#include <cassert>

using namespace std::literals;

int main()
{
    char c1 = 65;
    int8_t c2 = 127;
    uint8_t value = 0b0000'1111;

    unsigned char uc1 = 200;
    uint8_t uc2 = 200;
    int32_t i1 = 65000;
    uint64_t ui1 = 1'000'000;

    bool flag = true;
    flag = false;

    float fl1 = 3.14;
    double dl1 = 3.1415;
    long double dl3 = 3.1415;

    // auto
    auto x1 = 42; // int
    auto x2 = 42L; // long
    auto x3 = 42ULL; // unsigned long long
    auto d1 = 31.4; // double
    auto f1 = 31.4F; // float

    // c-string
    const char* cstr1 = "text";

    // std::string
    std::string str1 = "Hello";
    std::string str2 = "World";

    std::string text = str1 + " " + str2;
    std::cout << text << " - " << text.size() << " letters\n";

    auto ctext = "text"; // const char*
    auto text2 = "text"s; // since C++14 - std::string

    // init syntax
    int v1; // uninitialized value
    int v2 = 42;
    int v3(42);
    auto v4 = int(42);
    // since C++11
    int v5{5};
    char n1 = v5;
    //char n2{v5}; // ERROR - narrowing conversion

    int z1 = 0;
    int z2(0);
    //int z3(); // most vexing parse
    int z3 = int();
    z3 = 42;

    int z4{};
    assert(z4 == 0);

    int* ptr1 = NULL; // C++98 - deprecated
    int* ptr2 = nullptr; // C++11 - better option
    int* ptr3{};
}
