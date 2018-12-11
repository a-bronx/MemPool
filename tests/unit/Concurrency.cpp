#include <string>
#include <type_traits>
#include <array>
#include <vector>
#include "Mempool.h"
#include "gtest/gtest.h"
#include "Elements.h"

TEST(Mempool, AllocatesConcurrently)
{
    using namespace memory;
    using namespace std;

    using Element = size_t;
    auto pool = MemoryPool<Element>{};

    auto concurrency = 16;

    // concurrently allocate/deallocate a large number (qty) of elements in several iterations
    vector<std::thread> threads;
    for(size_t t = 0; t < concurrency; ++t)
    {
        threads.emplace_back([&]
        {
            auto qty = 100;
            vector<Element*> v;
            v.reserve(qty);

            auto iterations = 50;

            while (iterations --> 0)
            {
                v.clear();
                for(size_t i = 0; i < qty; ++i) {
                    auto el = pool.alloc(i);
                    ASSERT_NE(el, nullptr);
                    ASSERT_EQ(*el, i);
                    v.push_back(el);
                }

                for(size_t i = 0; i < qty; ++i) {
                    pool.free(v[i]);
                }
            }

        });
    }

    for(size_t t = 0; t < concurrency; ++t)
        threads[t].join();

    // everything must be deallocated
    ASSERT_EQ(pool.allocated(), 0);

}
