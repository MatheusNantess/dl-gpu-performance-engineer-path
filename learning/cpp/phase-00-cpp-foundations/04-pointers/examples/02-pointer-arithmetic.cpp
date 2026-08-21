/*
Example 02 — Pointer arithmetic

Complete, working program. Predict every address DIFFERENCE before running.

    g++ -std=c++17 -Wall -Wextra -g -O0 -o 02-pointer-arithmetic 02-pointer-arithmetic.cpp
    ./02-pointer-arithmetic
*/

#include <iostream>

int main() {
    int    ints[5]    = {10, 20, 30, 40, 50};
    double doubles[5] = {1.0, 2.0, 3.0, 4.0, 5.0};

    int*    pi = ints;        // array name decays: no & needed
    double* pd = doubles;

    std::cout << "--- +1 moves by sizeof(T), not by one byte ---\n";
    std::cout << "int*    base " << pi << "  +1 " << (pi + 1)
              << "  gap " << reinterpret_cast<const char*>(pi + 1)
                           - reinterpret_cast<const char*>(pi) << " bytes\n";
    std::cout << "double* base " << pd << "  +1 " << (pd + 1)
              << "  gap " << reinterpret_cast<const char*>(pd + 1)
                           - reinterpret_cast<const char*>(pd) << " bytes\n";

    std::cout << "\n--- four ways to say the same thing ---\n";
    std::cout << "ints[2]     = " << ints[2]     << "\n";
    std::cout << "*(ints + 2) = " << *(ints + 2) << "\n";
    std::cout << "pi[2]       = " << pi[2]       << "\n";
    std::cout << "*(pi + 2)   = " << *(pi + 2)   << "\n";
    // a[i] is DEFINED as *(a + i). These are the same expression.

    std::cout << "\n--- traversal, two forms ---\n";
    std::cout << "  by index:   ";
    for (int i = 0; i < 5; ++i) std::cout << ints[i] << " ";
    std::cout << "\n  by pointer: ";
    for (int* p = ints; p < ints + 5; ++p) std::cout << *p << " ";
    std::cout << "\n";
    // ints + 5 is one past the end. Forming that address is legal and is the
    // standard sentinel; dereferencing it is not.

    std::cout << "\n--- pointer difference counts ELEMENTS ---\n";
    int* a = &ints[1];
    int* b = &ints[4];
    std::cout << "b - a               = " << (b - a) << " elements\n";
    std::cout << "byte difference     = "
              << reinterpret_cast<const char*>(b)
                 - reinterpret_cast<const char*>(a) << " bytes\n";

    std::cout << "\n--- precedence ---\n";
    int* q = ints;
    std::cout << "*q + 1     = " << *q + 1     << "   (dereference, then add)\n";
    std::cout << "*(q + 1)   = " << *(q + 1)   << "   (advance, then dereference)\n";

    std::cout << "\n--- strided access: the coalescing question ---\n";
    // Contiguous: every element, in order.
    std::cout << "  stride 1: ";
    for (int i = 0; i < 5; ++i) std::cout << *(pi + i) << " ";
    std::cout << "\n";

    // Strided: every second element. On a CPU this wastes cache lines.
    // On a GPU, a warp doing this touches twice as many memory transactions
    // for the same useful data -- which is most of what memory optimization
    // is about.
    std::cout << "  stride 2: ";
    for (int i = 0; i < 5; i += 2) std::cout << *(pi + i) << " ";
    std::cout << "\n";

    return 0;
}