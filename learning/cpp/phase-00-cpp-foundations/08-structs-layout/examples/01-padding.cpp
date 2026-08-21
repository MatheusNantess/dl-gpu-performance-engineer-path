/*
Example 01 — Padding, seen directly

Complete, working program. PREDICT every sizeof before running.

    g++ -std=c++17 -Wall -Wextra -g -O0 -o 01-padding 01-padding.cpp
    ./01-padding
*/

#include <iostream>
#include <cstddef>   // offsetof

struct A { char x; int  y; };              // predict sizeof
struct B { int  y; char x; };              // predict sizeof
struct Bad  { char a; int b; char c; };    // predict sizeof
struct Good { int b; char a; char c; };    // predict sizeof
struct Point { float x, y, z; };           // predict sizeof
struct alignas(16) Aligned { float x, y, z, w; };

int main() {
    std::cout << "--- sizeof and alignof ---\n";
    std::cout << "A       size " << sizeof(A)     << "  align " << alignof(A)     << "\n";
    std::cout << "B       size " << sizeof(B)     << "  align " << alignof(B)     << "\n";
    std::cout << "Bad     size " << sizeof(Bad)   << "  align " << alignof(Bad)   << "\n";
    std::cout << "Good    size " << sizeof(Good)  << "  align " << alignof(Good)  << "\n";
    std::cout << "Point   size " << sizeof(Point) << "  align " << alignof(Point) << "\n";
    std::cout << "Aligned size " << sizeof(Aligned) << "  align " << alignof(Aligned) << "\n";

    std::cout << "\n--- where did the padding go? ---\n";
    std::cout << "A::x at offset " << offsetof(A, x) << "\n";
    std::cout << "A::y at offset " << offsetof(A, y) << "   <-- gap here is padding\n";

    std::cout << "\nBad::a at " << offsetof(Bad, a)
              << "  Bad::b at " << offsetof(Bad, b)
              << "  Bad::c at " << offsetof(Bad, c) << "\n";
    std::cout << "Good::b at " << offsetof(Good, b)
              << "  Good::a at " << offsetof(Good, a)
              << "  Good::c at " << offsetof(Good, c) << "\n";

    // Bad and Good hold identical data. Reordering fields largest-first
    // removed a third of the storage.

    std::cout << "\n--- wasted bytes ---\n";
    std::cout << "Bad  useful " << (sizeof(char)*2 + sizeof(int))
              << " bytes, actual " << sizeof(Bad)
              << ", wasted " << sizeof(Bad) - (sizeof(char)*2 + sizeof(int)) << "\n";
    std::cout << "Good useful " << (sizeof(char)*2 + sizeof(int))
              << " bytes, actual " << sizeof(Good)
              << ", wasted " << sizeof(Good) - (sizeof(char)*2 + sizeof(int)) << "\n";

    const long long n = 1000000;
    std::cout << "\nFor " << n << " elements:\n";
    std::cout << "  Bad  " << (n * static_cast<long long>(sizeof(Bad)))  / (1024*1024) << " MB\n";
    std::cout << "  Good " << (n * static_cast<long long>(sizeof(Good))) / (1024*1024) << " MB\n";

    // For a memory-bound kernel, that difference is wasted bandwidth --
    // bytes moved across the memory bus that carry no information.

    std::cout << "\n--- stride in an array of structs ---\n";
    Point points[4];
    std::cout << "sizeof(Point)        " << sizeof(Point) << "\n";
    std::cout << "stride between elements "
              << reinterpret_cast<const char*>(&points[1])
                 - reinterpret_cast<const char*>(&points[0]) << " bytes\n";
    std::cout << "stride between the x fields of consecutive points: "
              << reinterpret_cast<const char*>(&points[1].x)
                 - reinterpret_cast<const char*>(&points[0].x) << " bytes\n";

    // That last number is the AoS problem in one line. Reading only x from
    // every point walks memory with a stride of sizeof(Point), not
    // sizeof(float). On a GPU, 32 threads doing that do not coalesce.
    //
    // Storing all the x values in their own array -- SoA -- makes that
    // stride 4 again. See theory/03-aos-intro.md.

    std::cout << "\n--- alignas and vectorized loads ---\n";
    std::cout << "Aligned is " << sizeof(Aligned) << " bytes, aligned to "
              << alignof(Aligned) << "\n";
    std::cout << "base address " << static_cast<const void*>(&points[0]) << "\n";

    // CUDA's float4 is exactly this: 16 bytes, 16-byte aligned, readable in
    // one instruction that moves 16 bytes. The alignment is what makes the
    // wide load legal.

    return 0;
}