#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

template <typename TContainer>
void print(const std::string& desc, const TContainer& vec)
{
    std::cout << desc << ": ";

    for(const auto& item : vec)
        std::cout << item << " ";
    std::cout << "\n";
}

class MVector
{
public:
    typedef int* iterator;
    typedef const int* const_iterator;

    MVector(size_t size)
        : items_{new int[size]}, size_{size}
    {
        std::cout << "MVector(at " << items_ << ")\n";
        std::fill_n(begin(), size_, 0);
    }

    MVector(std::initializer_list<int> lst)
        : items_{new int[lst.size()]}, size_{lst.size()}
    {
        std::cout << "MVector(at " << items_ << ")\n";
        std::copy(lst.begin(), lst.end(), items_);
    }

    // copy constructor
    MVector(const MVector& source)
        : items_{new int[source.size()]}, size_{source.size()}
    {
        std::cout << "MVector(cc from " << source.items_ << " to " << items_ << ")\n";
        std::copy(source.begin(), source.end(), items_);
    }

    // copy assignment
    MVector& operator=(const MVector& source)
    {
        if (this != &source) // check for self-assignment
        {
            std::cout << "MVector operator=(ca: " << items_ << ")\n";
            delete[] items_;  // clean-up old state

            // copy
            items_ = new int[source.size()];
            size_ = source.size();
            std::copy(source.begin(), source.end(), items_);
        }

        return *this;
    }

    // move constructor
    MVector(MVector&& source) : items_{source.items_}, size_{source.size_}
    {
        std::cout << "MVector(mv " << items_ << ")\n";
        source.items_ = nullptr;
        source.size_ = 0;
    }

    // move assignment
    MVector operator=(MVector&& source)
    {
        if (this != &source)
        {
            delete[] items_;

            items_ = source.items_;
            size_ = source.size_;

            source.items_ = nullptr;
            source.size_ = 0;
        }

        return *this;
    }

    ~MVector() // destructor
    {
        std::cout << "~MVector(at " << items_ << ")\n";

        delete[] items_;
    }

    size_t size() const
    {
        return size_;
    }

    iterator begin()
    {
        return items_;
    }

    const_iterator begin() const
    {
        return items_;
    }

    iterator end()
    {
        return items_ + size_;
    }

    const_iterator end() const
    {
        return items_ + size_;
    }

    int& operator[](size_t index)
    {
        return items_[index];
    }

    const int& operator[](size_t index) const
    {
        return items_[index];
    }
private:
    int* items_;
    size_t size_;
};

TEST_CASE("MVector")
{
    SECTION("construction with size")
    {
        MVector vec(5);

        REQUIRE(vec.size() == 5);

        REQUIRE(std::all_of(vec.begin(), vec.end(), [](int x) { return x == 0; }));
    }

    SECTION("construction with list")
    {
        MVector vec = {1, 2, 3, 4, 5};

        REQUIRE(vec.size() == 5);
        REQUIRE(vec[1] == 2);
        REQUIRE(vec[4] == 5);
    }

    SECTION("indexing")
    {
        MVector vec(10);

        vec[0] = 1;
        REQUIRE(vec[0] == 1);

        print("vec", vec);
    }

    SECTION("copying")
    {
        MVector vec1 = {1, 2, 3, 4, 5};
        MVector vec2 = vec1; // copy constructor

        REQUIRE(vec2[3] == 4);

        MVector vec3 = {6, 6, 5};
        vec2 = vec3; // copy assignment

        vec3 = vec3;
    }

    SECTION("moving")
    {
        MVector vec1 = {1, 2, 3, 4, 5};
        MVector vec2 = std::move(vec1); // move construct

        REQUIRE(vec2.size() == 5);
        REQUIRE(vec2[1] == 2);
        REQUIRE(vec1.size() == 0);

        vec1 = std::move(vec2); // move assigment
    }
}

TEST_CASE("dynamic memory allocation")
{
    SECTION("c-style")
    {
        void* memory = malloc(sizeof(double));
        double* ptr = static_cast<double*>(memory);

        *ptr = 3.14;

        free(memory);
    }

    SECTION("low-level C++")
    {
        double* ptr = new double(3.14);

        REQUIRE(*ptr == 3.14);

        delete ptr; // delete for objects
    }

    SECTION("low-level C++ - arrays")
    {
        double* tab = new double[100];

        tab[7] = 3.14;
        REQUIRE(tab[7] == 3.14);

        delete[] tab; // delete for arrays
    }
}
