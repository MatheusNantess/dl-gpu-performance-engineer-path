/*
Example 01 — Contiguity, seen directly

Complete, working program. Read it, predict the address DIFFERENCES
(not the absolute values -- those change every run), then run it.

    g++ -std=c++17 -Wall -Wextra -g -O0 -o 01-contiguity 01-contiguity.cpp
    ./01-contiguity
*/

#include <iostream>

int main() {
    int    ints[5]    = {10, 20, 30, 40, 50};
    float  floats[5]  = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    double doubles[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
    char   chars[5]   = {'a', 'b', 'c', 'd', 'e'};

    std::cout << "--- int[5], sizeof(int) = " << sizeof(int) << " ---\n";
    for (int i = 0; i < 5; ++i) {
        std::cout << "  ints[" << i << "]  addr " << static_cast<const void*>(&ints[i])
                  << "   value " << ints[i] << "\n";
    }

    // The interesting number is not the address. It is the DIFFERENCE.
    std::cout << "\n  byte gap between consecutive elements: "
              << reinterpret_cast<const char*>(&ints[1])
                 - reinterpret_cast<const char*>(&ints[0]) << "\n";

    std::cout << "\n--- stride per type ---\n";
    std::cout << "  int    stride: "
              << reinterpret_cast<const char*>(&ints[1])    - reinterpret_cast<const char*>(&ints[0])    << "\n";
    std::cout << "  float  stride: "
              << reinterpret_cast<const char*>(&floats[1])  - reinterpret_cast<const char*>(&floats[0])  << "\n";
    std::cout << "  double stride: "
              << reinterpret_cast<const char*>(&doubles[1]) - reinterpret_cast<const char*>(&doubles[0]) << "\n";
    std::cout << "  char   stride: "
              << reinterpret_cast<const char*>(&chars[1])   - reinterpret_cast<const char*>(&chars[0])   << "\n";

    // The stride always equals sizeof(T). That is not a coincidence -- it is
    // the definition of contiguous storage, and it is what makes
    //     address_of(a[i]) == address_of(a[0]) + i * sizeof(T)
    // true, which in turn is what pointer arithmetic relies on.

    std::cout << "\n--- sizeof on the array itself ---\n";
    std::cout << "  sizeof(ints)          " << sizeof(ints) << "\n";
    std::cout << "  sizeof(ints[0])       " << sizeof(ints[0]) << "\n";
    std::cout << "  element count         " << sizeof(ints) / sizeof(ints[0]) << "\n";
    std::cout << "  sizeof(doubles)       " << sizeof(doubles) << "\n";

    // Note that ints and doubles have the same element COUNT but different
    // byte totals. For a memory-bound kernel, bytes are what cost time.

    std::cout << "\n--- the whole array occupies one unbroken block ---\n";
    std::cout << "  first byte: " << static_cast<const void*>(&ints[0]) << "\n";
    std::cout << "  last byte:  " << static_cast<const void*>(&ints[4]) << "\n";
    std::cout << "  span:       "
              << reinterpret_cast<const char*>(&ints[4])
                 - reinterpret_cast<const char*>(&ints[0]) + static_cast<long>(sizeof(int))
              << " bytes\n";

    // On a GPU, 32 threads reading 32 consecutive elements of an array like
    // this touch one unbroken block of bytes, and the hardware fetches it in
    // a few wide transactions. That is coalescing, and this contiguity is
    // the entire reason it is possible.

    return 0;
}