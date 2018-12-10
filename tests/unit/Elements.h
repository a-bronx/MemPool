#ifndef MEMPOOL_TEST_ELEMENTS_H
#define MEMPOOL_TEST_ELEMENTS_H

// Different kinds of classes to use as MemoryPool<> elements.

struct DefaultConstructible
{
    int I;
    std::string S;

    // a non-default constructor to test ability of MemPoool to handle
    // non-default-constructible classes.
    DefaultConstructible() noexcept
        : I(10), S("Default")
    {}
};

struct CustomConstructible
{
    int I;
    std::string S;

    // a non-default constructor to test ability of MemPoool to handle
    // non-default-constructible classes.
    CustomConstructible(int i, std::string s) noexcept
        : I(i), S(std::move(s))
    {}
};

struct ThrowConstructible
{
    // a constructor throwing an excetption
    ThrowConstructible() { throw std::runtime_error("Thrown"); }
};


#endif // MEMPOOL_TEST_ELEMENTS_H
