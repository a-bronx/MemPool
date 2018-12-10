#include <string>
#include <type_traits>
#include <array>
#include <vector>
#include "Mempool.h"
#include "gtest/gtest.h"

TEST(Mempool, AllocatesConcurrently)
{
    using namespace memory;
    using namespace std;

    auto pool = MemoryPool<int, 1024>{};

    auto concurrency = 16;

    // concurrently allocate/deallocate a large number (qty) of elements in several iterations
    vector<std::thread> threads;
    for(size_t t = 0; t < concurrency; ++t)
    {
        threads.emplace_back([&]
        {
            auto qty = 1'000;
            vector<int*> v;
            v.reserve(qty);

            auto iterations = 1'000;

            while (iterations --> 0)
            {
                v.clear();
                for(size_t i = 0; i < qty; ++i) {
                    auto el = pool.alloc();
                    ASSERT_NE(el, nullptr);
                    v.push_back(el);
                }

                for(size_t i = 0; i < qty; ++i)
                    pool.free(v[i]);
            }

        });
    }

    for(size_t t = 0; t < concurrency; ++t)
        threads[t].join();

    // everything must be deallocated
    ASSERT_EQ(pool.allocated(), 0);

}
