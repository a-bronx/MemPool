DISCLAIMER: This code is a playground, it was not thorougly tested and most probably contains some nasty errors. DO NOT USE IT IN PRODUCTION!

**Table of Contents**
<!-- TOC -->

- [MemPool](#mempool)
    - [Details](#details)
        - [Interface](#interface)
    - [How to build and run tests](#how-to-build-and-run-tests)
        - [Disclaimer](#disclaimer)
        - [Toolset](#toolset)
        - [Build steps](#build-steps)

<!-- /TOC -->

# MemPool

This header-only C++17 library provides an implementation of
a simple concurent lock-free memory pool of elements of a single type.
The element type is passed as a first template parameter.

## Details


### Interface

```cpp
template 
    < typename ElemT
    , unsigned short ChunkSize = 65534
    , typename Allocator = std::allocator<ElemT>
    >
class MemoryPool final
{
public:
    // construction
    MemoryPool();
    MemoryPool(Allocator& alloc);
    explicit MemoryPool(size_t initialCapacity, Allocator& alloc = Allocator());
    
    ~MemoryPool();

    // the pool is not copyable and not movable
    MemoryPool(const MemoryPool&) = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;
    MemoryPool(MemoryPool&& other) = delete;
    MemoryPool& operator=(MemoryPool&&) = delete;

    // allocation (concurrent, lock-free[*])
    template<typename... Args> [[nodiscard]] 
    ElemT* alloc(Args&& ...args);

    // deallocation (concurrent, lock-free)
    void free(ElemT* elem) noexcept;

    // diagnostics (non-concurrent)
    static size_t chunk_size() noexcept;
    size_t chunk_count() const noexcept;
    size_t capacity() const noexcept;
    size_t allocated() const noexcept;
};

```

The memory pool is implemented as a single-linked list of fixed size chunks,
where each of them contains a preallocated arena of elements.
The number of elements in each chunk is passed as a second template parameter.
The maximum (and default) chunk size if 65534 elements.
The minimum chunk size is 2 elements, but you should avoid too small chunks 
as they have relatively large overhead.

When the pool's current capacity is exceeded, it automatically grows adding 
a new chunk. The pool never shrinks until being destroyed.

Arena chunks are allocated using a standard allocator. 
A custom allocator can be passed as a third template parameter.

Concurrency is supported on a chunk level by a lock-free free list 
(a ring buffer of indices).

[*] NOTE: allocation of a new chunk for pool growth is not lock-free, 
though it should happen not very often, and only once per a new chunk allocation.

The pool does not require the element type to be default constructible.
The pool's `alloc(Args&&...)` method uses a variadic template to pass constructor 
parameters and returns a fully constructed element (or throws).
The pool's `free()` method calls an element't destructor (the destructor must be `noexcept`).

Example:

```cpp
#include "mempool.h"

class Widget { 
public:
    explicit Widget(std::string name);
    ...
};


...

// create a pool with a chunk size 1024, and a capacity reserved for 4096 elements (i.e. 4 chunks)
auto pool = MemoryPool<Shape, 1024> { 4096 };

Widget* widget = pool.alloc("Hello World!"s); // will call Widged(std::string)
...
pool.free(widget); // will call Widget::~Widget();

```

---

## How to build and run tests

The library is header-only and does not require a build.

Unit tests (Google Test) can be built using CMake. 
It seems Google Test is not well integrated with MSBuild yet, hence
no traditional `.vcxproj/.sln` are provided.


### Disclaimer

> This is my first experince using CMake and Google Test/Benchmark
for Visual Studio, and Visual Studio's support of them is still flaky.
Within a limited time frame for the project, I was not able to make Google 
Benchmark to automatically download with dependencies and build in VS,
so the project uses Google Test only.*

### Toolset

The project was created with the Microsoft Visual Studio 2017 v15.9.3.

Following Visual Studio 20017 components must be installed:

* Visual C++ tools for CMake and Linux
* Visual C++ for Linux Development
* Windows SDK 10 for Desktop [x86 and x64]

### Build steps

To open the project in Visual Studio, run:

```cmd
> cd MemPool
> devenv .
```

or start Visual Studio and use the `FILE -> Open -> Folder` main menu command.

When project is opened, select the `x64-Release` build configuration.

To configure the project and make InteliSense working, regenerate CMake 
cache when Visual Studio asks it or with the 
`CMAKE -> Cache -> Generate [x64-Release] -> MemPool` main menu command.
Note that configuraton will download the Google Test library from 
their Gir repository.

To build the project use the `CMAKE -> [Re]Build All` main menu command, 
or right-click on a `CMakeLists.txt` and select `Build` menu command from 
a popup menu.

To run tests, right-click on `tests/unit/UnitTest.cpp` and select 
`Set as startup item`. Then run the `UnitTest` with the 
`DEBUG -> Start [Without Debugging]` main menu command (F5 or Ctrl-F5).




