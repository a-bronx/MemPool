cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
#add_test (UnitTest "UnitTest.cpp" "../src/Mempool.h")
add_executable (UnitTest 
	"UnitTest.cpp" 
	"Basics.cpp" 
	"Concurrency.cpp" 
)

target_link_libraries(UnitTest MemPool gtest_main)

# TODO: Add tests and install targets if needed.
set_property(TARGET UnitTest PROPERTY CXX_STANDARD 17)
set_property(TARGET UnitTest PROPERTY CXX_STANDARD_REQUIRED ON)
