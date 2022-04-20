#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

int main()
{
    //const double pi = 3.1415; // west-const
    double const pi = 3.1415; // east-const

    const int n = 100;

    const auto company_name = "Radmor"s;
    const std::string ver = "ver. 1.0";

    //company_name[0] = "r";
    //company_name = "WB Group";


    // const + pointers

    // case 1 - pointer to const
    int x = 42;
    int y = 1024;

    const int* ptr = &x; // pointer to const
    //*ptr = 665; // ERROR
    ptr = &y;

    // case 2 - const pointer
    int* const cptr = &x;
    *cptr = 665;

    //cptr = &y; // ERROR

    // case 3 - const pointer to const
    const int* const cptrc = &x;
    //int const* const cptrc = &x // east-const
    //(*cptrc) = 888; // ERROR
    //cptrc = &y; // ERROR

    const char* cstring1 = "text";
    const char* cstring2 = "text";

    cstring1 = "ala ma kota";

 }
