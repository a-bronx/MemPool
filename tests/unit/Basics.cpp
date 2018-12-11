#include <string>
#include <type_traits>
#include <array>
#include <vector>
#include "Mempool.h"
#include "gtest/gtest.h"
#include "Elements.h"

#define DEFAULT_CHUNK_SIZE 65534


TEST(Mempool, ConstructsWithDefaultCapacity)
{
    using namespace memory;
    { 
        auto pool = MemoryPool<char> {};
        ASSERT_EQ(pool.chunk_size(), DEFAULT_CHUNK_SIZE);
        ASSERT_EQ(pool.chunk_count(), 1);
        ASSERT_EQ(pool.allocated(), 0);
        ASSERT_EQ(pool.capacity(), pool.chunk_size());
        ASSERT_EQ(pool.allocated(), 0);
    }
    { 
        auto pool = MemoryPool<int> {};
        ASSERT_EQ(pool.chunk_size(), DEFAULT_CHUNK_SIZE);
        ASSERT_EQ(pool.chunk_count(), 1);
        ASSERT_EQ(pool.allocated(), 0);
        ASSERT_EQ(pool.capacity(), pool.chunk_size());
        ASSERT_EQ(pool.allocated(), 0);
    }
    { 
        auto pool = MemoryPool<unsigned long> {};
        ASSERT_EQ(pool.chunk_size(), DEFAULT_CHUNK_SIZE);
        ASSERT_EQ(pool.chunk_count(), 1);
        ASSERT_EQ(pool.allocated(), 0);
        ASSERT_EQ(pool.capacity(), pool.chunk_size());
        ASSERT_EQ(pool.allocated(), 0);
    }
    { 
        auto pool = MemoryPool<DefaultConstructible> {};
        ASSERT_EQ(pool.chunk_size(), DEFAULT_CHUNK_SIZE);
        ASSERT_EQ(pool.chunk_count(), 1);
        ASSERT_EQ(pool.allocated(), 0);
        ASSERT_EQ(pool.capacity(), pool.chunk_size());
        ASSERT_EQ(pool.allocated(), 0);
    }
    { 
        auto pool = MemoryPool<CustomConstructible> {};
        ASSERT_EQ(pool.chunk_size(), DEFAULT_CHUNK_SIZE);
        ASSERT_EQ(pool.chunk_count(), 1);
        ASSERT_EQ(pool.allocated(), 0);
        ASSERT_EQ(pool.capacity(), pool.chunk_size());
        ASSERT_EQ(pool.allocated(), 0);
    }
    { 
        auto pool = MemoryPool<ThrowConstructible> {};
        ASSERT_EQ(pool.chunk_size(), DEFAULT_CHUNK_SIZE);
        ASSERT_EQ(pool.chunk_count(), 1);
        ASSERT_EQ(pool.allocated(), 0);
        ASSERT_EQ(pool.capacity(), pool.chunk_size());
        ASSERT_EQ(pool.allocated(), 0);
    }
}

TEST(Mempool, ConstructsWithCustomCapacity)
{
    size_t capacities[] = { 2, 10, 1'000, 10'000, 100'000, 1'000'000 };

    // default chunk size
    for(auto cap : capacities)
    {
        auto pool = memory::MemoryPool<int> { cap };
        ASSERT_EQ(pool.chunk_size(), DEFAULT_CHUNK_SIZE);
        ASSERT_EQ(pool.chunk_count(), cap/pool.chunk_size() + 1);
        ASSERT_EQ(pool.allocated(), 0);
        ASSERT_EQ(pool.capacity(), pool.chunk_count() * pool.chunk_size());
        ASSERT_EQ(pool.allocated(), 0);
    }

    // custom chunk size
    for(auto cap : capacities)
    {
        auto pool = memory::MemoryPool<int, 1024> { cap };
        ASSERT_EQ(pool.chunk_size(), 1024);
        ASSERT_EQ(pool.chunk_count(), cap/pool.chunk_size() + 1);
        ASSERT_EQ(pool.allocated(), 0);
        ASSERT_EQ(pool.capacity(), pool.chunk_count() * pool.chunk_size());
        ASSERT_EQ(pool.allocated(), 0);
    }

}

TEST(Mempool, SupportsDefaultConstructibleElements)
{
    auto pool = memory::MemoryPool<DefaultConstructible> {};

    auto elem = pool.alloc();

    ASSERT_NE(elem, nullptr);
    ASSERT_EQ(elem->I, 10);
    ASSERT_EQ(elem->S, "Default");
    pool.free(elem);
    ASSERT_EQ(pool.allocated(), 0);
}

TEST(Mempool, SupportsCustomConstructibleElements)
{
    auto pool = memory::MemoryPool<CustomConstructible> {};

    auto elem = pool.alloc(123, "Custom");

    ASSERT_NE(elem, nullptr);
    ASSERT_EQ(elem->I, 123);
    ASSERT_EQ(elem->S, "Custom");
    pool.free(elem);
    ASSERT_EQ(pool.allocated(), 0);
}

TEST(Mempool, HandlesThrowngConstructors)
{
    auto pool = memory::MemoryPool<ThrowConstructible> {};

    ASSERT_THROW(pool.alloc(), std::runtime_error);
    ASSERT_EQ(pool.allocated(), 0);
}

TEST(Mempool, AllocatesAndDeallocates)
{
    auto pool = memory::MemoryPool<int>{};

    auto constexpr qty = 1'000'000;

    std::vector<int*> v;
    v.reserve(qty);
    for(size_t i = 0; i < qty; ++i) {
        auto el = pool.alloc();
        ASSERT_NE(el, nullptr);
        v.push_back(el);
    }

    ASSERT_EQ(pool.allocated(), qty);

    for(size_t i = 0; i < qty; ++i)
        pool.free(v[i]);

    ASSERT_EQ(pool.allocated(), 0);
}

TEST(Mempool, DoesNotFailOnFreeingGarbage)
{
    auto pool = memory::MemoryPool<int>{};

    auto ourElement = pool.alloc();
    ASSERT_EQ(pool.allocated(), 1);

    pool.free(nullptr);
    ASSERT_EQ(pool.allocated(), 1);

    int garbage;
    pool.free(&garbage);
    ASSERT_EQ(pool.allocated(), 1);
    
    pool.free(ourElement);
    ASSERT_EQ(pool.allocated(), 0);
}
